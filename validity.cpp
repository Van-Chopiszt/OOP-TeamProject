#include "validity.h"
#include <cmath>
using namespace std;

void SBWDResult::append(const SBWD& res) {
	resarr.push_back(res);
	++size;
}

double SBWDResult::ave_S() {
	double sum = 0.0;
	for (int i = 0; i < size; ++i) {
		sum += resarr[i].sample;
	}
	return sum / size;
}

double SBWDResult::ave_B() {
	double sum = 0.0;
	for (int i = 0; i < size; ++i) {
		sum += resarr[i].buffer;
	}
	return sum / size;
}

double SBWDResult::ave_W() {
	double sum = 0.0;
	for (int i = 0; i < size; ++i) {
		sum += resarr[i].waste;
	}
	return sum / size;
}

double SBWDResult::ave_D() {
	double sum = 0.0;
	for (int i = 0; i < size; ++i) {
		sum += resarr[i].dilution;
	}
	return sum / size;
}

ostream& operator<<(ostream& os, SBWDResult& R) {
	os << "Average sample droplets consumed: " << R.ave_S() << endl
		<< "Average buffer droplets consumed: " << R.ave_B() << endl
		<< "Average droplets wasted: " << R.ave_W() << endl
		<< "Average number of dilution operations: " << R.ave_D() << endl;
	return os;
}

DropletTracker::DropletTracker(int d_): d(d_),oprt(0),onboard(0) {
	int ccs = 1;
	for (int i = 1; i <= d; ++i) {
		ccs *= 2;
	}
	dr = new int[ccs + 1];

	for (int i = 0; i <= ccs; ++i)
		dr[i] = 0;

	p2 = new int[d + 1]; //p2[j]=2^j
	p2[0] = 1;
	for (int i = 1; i <= d; i++) {
		p2[i] = p2[i - 1] * 2;
	}
}

DropletTracker::DropletTracker(const SPInput& I): oprt(0), onboard(0) {
	d = I.d;
	int ccs = 1;
	for (int i = 1; i <= d; ++i) {
		ccs *= 2;
	}
	dr = new int[ccs + 1];

	for (int i = 0; i <= ccs; ++i)
		dr[i] = 0;

	p2 = new int[d + 1]; //p2[j]=2^j
	p2[0] = 1;
	for (int i = 1; i <= d; i++) {
		p2[i] = p2[i - 1] * 2;
	}
}

DropletTracker::~DropletTracker() {
	delete[]dr;
	delete[]p2;
}

bool DropletTracker::match(const SPInput& I) {
	int t = I.TC.size();
	for (int i = 0; i < t; ++i) {
		if (dr[I.TC[i]] < I.S[i]) {
			return false;
		}
	}
	return true;
}

bool check(const SPInput& I, Node** NF, const Node& Source, SBWD& result) {
	int d = I.d;
	DropletTracker DT(d);
	DT.process(Source);

	result.sample = DT[DT.p2[d]];
	result.buffer = DT[0];
	int sum = 0;
	for (int i = 0; i < I.S.size(); ++i) {
		sum += I.S[i];
	}
	result.waste = result.sample + result.buffer - sum;
	
	for (int i = 0; i < d; ++i) {
		for (int j = 0; j < DT.p2[i] + 1; ++j) {
			DT.process(NF[i][j]);
		}
		//cout << "After round " << i + 1 << ":" << endl;
		DT.print();
		cout << endl;
	}

	result.dilution = DT.getopr();

	return DT.match(I);

}

void DropletTracker::process(const Node& node) {
	if (node.type == SRC) {
		int t = node.to.size();
		for (int i = 0; i < t; ++i) {
			if (node.to[i].f.get(GRB_DoubleAttr_X) != 0) {
				int c = node.to[i].end.cc;
				int f = node.to[i].f.get(GRB_DoubleAttr_X);
				dr[c] = f;
			}			
		}
	}
	else {
		int t = node.to.size();
		for (int i = 0; i < t; ++i) {
			int f = round(node.to[i].f.get(GRB_DoubleAttr_X));
			if (f != 0) {
				int ec = node.to[i].end.cc;
				int sc = node.to[i].start.cc;
				if (sc < ec) {
					if (sc == 0) {
						//cout << "Release " << f << " buffer droplet(s)" << endl;
						++onboard;
					}
					if (2 * ec - sc == p2[d]) {
						//cout << "Release " << f << " sample droplet(s)" << endl;
						++onboard;
					}
					dr[ec] += 2 * f;
					dr[sc] -= f;
					dr[2 * ec - sc] -= f;
					++oprt;
					/*cout << f << " * " << sc << " + " << f << " * " << 2 * ec - sc
						<< " = " << 2 * f << " * " << ec << ", Operation " << oprt << endl;*/
				}
			}
		}
	}
}

void DropletTracker::process(Node** NF, const Node& Source, vector<StepInfo>& cmd) {
	int t = Source.to.size();
	for (int i = 0; i < t; ++i) {
		if (Source.to[i].f.get(GRB_DoubleAttr_X) != 0) {
			int c = Source.to[i].end.cc;
			int f = round(Source.to[i].f.get(GRB_DoubleAttr_X));
			dr[c] = f;
		}
	}

	for (int i = 0; i < d; ++i) {
		for (int j = 0; j < p2[i] + 1; ++j) {
			int t = NF[i][j].to.size();
			for (int k = 0; k < t; ++k) {
				if (NF[i][j].to[k].f.get(GRB_DoubleAttr_X) != 0) {
					int ec = NF[i][j].to[k].end.cc;
					int sc = NF[i][j].to[k].start.cc;
					if (sc < ec) {
						StepInfo tmp;
						int f = round(NF[i][j].to[k].f.get(GRB_DoubleAttr_X));
						if (sc == 0) {
							//cout << "Release " << f << " buffer droplet(s)" << endl;
							tmp.buffer = true;
							++onboard;
						}
						if (2 * ec - sc == p2[d]) {
							//cout << "Release " << f << " sample droplet(s)" << endl;
							tmp.sample = true;
							++onboard;
						}
						tmp.merge1 = sc;
						tmp.merge2 = 2 * ec - sc;

						int tt = NF[d][ec].to.size();
						int wst = 0;
						for (int l = 0; l < tt; ++l) {
							if (NF[d][ec].to[l].end.type == WST && NF[d][ec].to[l].f.get(GRB_DoubleAttr_X) != 0)
								wst = round(NF[d][ec].to[l].f.get(GRB_DoubleAttr_X));
						}
						for (int l = 0; l < f; ++l) {
							if (l < wst) {
								tmp.waste = ec;
								cmd.push_back(tmp);
							}
							else {
								tmp.waste = -1;
								cmd.push_back(tmp);
							}
						}

						dr[ec] += 2 * f;
						dr[sc] -= f;
						dr[2 * ec - sc] -= f;
						++oprt;
						/*cout << f << " * " << sc << " + " << f << " * " << 2 * ec - sc
							<< " = " << 2 * f << " * " << ec << ", Operation " << oprt << endl;
						if (wst)
							cout << "Sink " << wst << " droplet(s) of concentration " << ec << endl;*/
					}
				}
			}
		}
	}
}

void print(const SBWD& a) {
	cout << "Sample droplets consumed: " << a.sample << endl
		<< "Buffer droplets consumed: " << a.buffer << endl
		<< "Droplets wasted: " << a.waste << endl
		<< "Number of operations: " << a.dilution << endl;
}

void DropletTracker::print() {
	for (int i = 1; i < p2[d]; ++i) {
		if (dr[i]) {
			cout << i << ":" << dr[i] << " ";
		}
	}
	//cout << "Total droplets onboard: " << onboard << endl;
}