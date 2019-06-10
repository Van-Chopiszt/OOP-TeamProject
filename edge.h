#ifndef EDGE_H
#define EDGE_H

#include "gurobi_c++.h"
#include <string>
using std::string;

enum Type { SRC, ORD, SNK, WST }; //source, ordinary, sink, waste

class Node;

class Edge {
public:
	GRBVar f;
	struct NodeInfo {
		Type type;
		int lv;
		int cc;
	}start, end;
	Edge(const Node* u, const Node* v);
	Edge();
	string name() const;
};

#endif //EDGE_H
