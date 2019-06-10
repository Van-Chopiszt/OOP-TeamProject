#include "adjust.h"
using namespace std;

void SPAdjust(const SPInput& I, GRBModel* model, Node** NF, Node& Source, Node& Sink, Node& Waste) {
	int d = I.d;
	int p2d = 1;
	for (int i = 1; i <= d; ++i) {
		p2d *= 2;
	}

	for (int j = 0; j <= p2d; ++j) {
		model->remove(model->getConstrByName(NF[d][j].name()));
	}

	for (int i = 0; i <= p2d; ++i) {
		int t = NF[d][i].to.size();
		if (t == 2) {
			model->remove(model->getConstrByName(NF[d][i].to.back().name()));
			Sink.in.remove(model->getVarByName(NF[d][i].to.back().name()));
			NF[d][i].out.remove(model->getVarByName(NF[d][i].to.back().name()));
			model->remove(model->getVarByName(NF[d][i].to.back().name()));
			NF[d][i].to.resize(1);
		}
	}

	int t = I.TC.size();
	for (int i = 0; i < t; ++i) {
		NF[d][I.TC[i]].addEdge(Sink, model);
		model->addConstr(NF[d][I.TC[i]].to.back().f == I.S[i], NF[d][I.TC[i]].to.back().name());
	}

	for (int j = 0; j <= p2d; ++j) {
		model->addConstr(NF[d][j].in == NF[d][j].out, NF[d][j].name());
	}
}