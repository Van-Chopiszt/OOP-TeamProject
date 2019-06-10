#include <iostream>
#include <fstream>
#include <ctime>
#include "gurobi_c++.h"
#include "settings.h"
#include "node.h"
#include "validity.h"
#include "randominput.h"
#include "adjust.h"
#include "Command.h"
#include "Visualizer.h"
using namespace std;

int main()
{
	srand((unsigned int)time(0));
	
	try {
		GRBEnv env = GRBEnv(true);
		env.set("LogFile", "SP.log");
		env.start();

		int d,tn;
		cout << "d=? (As the method proposed by the paper requires huge amount of computation, we suggest the value of d no more than 9)" << endl;
		cin >> d;
		cout << "Number of target concentrations? (No more than 3)" << endl;
		cin >> tn;
		int m, n;
		cout << "The size of the chip: m=? n=?" << endl;
		cin >> m >> n;

		int* p2 = new int[d + 1]; //p2[j]=2^j
		p2[0] = 1;
		for (int i = 1; i <= d; i++) {
			p2[i] = p2[i - 1] * 2;
		}

		GRBModel model = GRBModel(env);

		Node** NF = new Node * [d + 1];
		Node Source(-1, 0, SRC);
		Node Waste(d + 1, -1, WST);
		Node Sink(d + 1, -2, SNK);

		for (int i = 0; i < 100; ++i) {
			SPInput I=random(d,tn,WASTE);
			//SPInput I(7, 1, 1, { 96,9,43 }, { 1,1,1 });
			//SPInput I(7, 1, 1, { 36,81,93 }, { 1,1,1 });
			//SPInput I(7, 1, 1, { 23,26,64,109 }, { 1,1,1,1 });

			if (i == 0) {
				SPBuild(I, &model, NF, Source, Sink, Waste);
			}				
			else {
				SPAdjust(I, &model, NF, Source, Sink, Waste);
			}
						
			model.optimize();

			/*SBWD result;

			if (check(I, NF, Source, result)) {
				cout << "Sample preparation is correct." << endl;
			}
			else
				cout << "Sample preparation is wrong." << endl;*/

			DropletTracker D(d);
			vector<StepInfo> stepinfo;
			D.process(NF, Source, stepinfo);
			/*int cmdnum = stepinfo.size();
			cout << cmdnum << endl;
			for (int x = 0; x < cmdnum; ++x) {
				cout << "Command " << x + 1 << ":\n";
				if (stepinfo[x].buffer) cout << "Release buffer droplet\n";
				if (stepinfo[x].sample) cout << "Release sample droplet\n";
				cout << "Merge " << stepinfo[x].merge1 << " and " << stepinfo[x].merge2 << endl;
				if (stepinfo[x].waste >= 0) cout << "Sink a droplet of concentration " << stepinfo[x].waste << endl;
			}*/
			cout << "Target concentration: " << endl;
			for (int i = 0; i < I.TC.size(); ++i) {
				cout << I.TC[i] << " ";
			}
			cout << endl;
			
			Command cmd(m, n, d);
			cmd.initialize(stepinfo);
			Chip chip(m, n);
			Visualizer vis;
			while (cmd.update(chip)) {
				//vis.draw(chip);
				cout << chip << endl;
			}
		}

		for (int j = 0; j < d + 1; ++j) {
			delete[]NF[j];
		}
		delete[]NF;
		
		delete[]p2;
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (...) {
		cout << "Exception during optimization" << endl;
	}

	return 0;
}