#ifndef ADJUST_H
#define ADJUST_H

#include "node.h"
#include "settings.h"
#include "gurobi_c++.h"

void SPAdjust(const SPInput& I, GRBModel* model, Node** NF, Node& Source, Node& Sink, Node& Waste);
//make adjustments to the top-level nodes so that a new model can be generated faster

#endif //ADJUST_H