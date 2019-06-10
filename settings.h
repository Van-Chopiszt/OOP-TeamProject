#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>
#include "gurobi_c++.h"
#include "edge.h"
#include "node.h"
using std::vector;

const int STprecision = 6; //precision for single-target sample preparation

const int STsampcc = 64; //concentration of sample droplet under STprecision
						   //important: must be modified together with STprecision

const int MTprecision = 10;

enum Mode { SAMPLE, WASTE };

class SPInput {
public:
	int d;
	int costs;
	int costb;
	vector<int> TC;
	vector<int> S;
	SPInput(int d_, int cs, int cb, vector<int> tc, vector<int> s) :
		d(d_), costs(cs), costb(cb), TC(tc), S(s) {};
	SPInput() : d(0), costs(0), costb(0) {};
	SPInput(const SPInput& I);
};

void SPBuild(const SPInput& I, GRBModel* model, Node** NF, Node& Source, Node& Sink, Node& Waste);

#endif //SETTINGS_H