#include "settings.h"
#include "gurobi_c++.h"
#include <sstream>
using namespace std;

void SPBuild(const SPInput& I, GRBModel* model, Node** NF, Node& Source, Node& Sink, Node& Waste) {
	int d = I.d;
	int* p2 = new int[d + 1]; //p2[j]=2^j
	p2[0] = 1;
	for (int i = 1; i <= d; i++) {
		p2[i] = p2[i - 1] * 2;
	}

	for (int i = 0; i <= d; ++i) {
		NF[i] = new Node[p2[i] + 1];
		int step = p2[d - i];
		for (int j = 0; j < p2[i] + 1; ++j) {
			NF[i][j].set(i, j * step);
		}
	}

	GRBVar buffer = Source.addEdge(NF[0][0], model);
	GRBVar sample = Source.addEdge(NF[0][1], model);

	for (int i = 0; i < d; ++i) {
		for (int j = 0; j < p2[i] + 1; ++j) {
			NF[i][j].addEdge(NF[i + 1][j * 2], model);
			for (int k = j + 1; k < p2[i] + 1; k+=2) {
				NF[i][j].addEdge(NF[i + 1][j + k], model);
				NF[i][k].addEdge(NF[i + 1][j + k], model);
				model->addConstr(NF[i][j].to.back().f - NF[i][k].to.back().f == 0,
					NF[i][j].to.back().name() + "=" + NF[i][k].to.back().name());
			}
		}
	}

	for (int j = 0; j <= p2[d]; ++j) {
		NF[d][j].addEdge(Waste, model);
	}

	int t = I.TC.size();
	for (int i = 0; i < t; ++i) {
		NF[d][I.TC[i]].addEdge(Sink, model);
		model->addConstr(NF[d][I.TC[i]].to.back().f == I.S[i], NF[d][I.TC[i]].to.back().name());
	}

	for (int i = 0; i <= d; ++i) {
		for (int j = 0; j < p2[i] + 1; ++j) {
			model->addConstr(NF[i][j].in == NF[i][j].out, NF[i][j].name());
		}
	}

	model->setObjective(I.costs * sample + I.costb * buffer, GRB_MINIMIZE);

	delete[]p2;
}

SPInput::SPInput(const SPInput& I) :
	d(I.d), costs(I.costs), costb(I.costb), TC(I.TC), S(I.S) {};