#ifndef NODE_H
#define NODE_H

#include <vector>
#include "gurobi_c++.h"
#include "edge.h"
using std::vector;

class Node {
	friend Edge;
private:
	int level;
	int concentration;
public:
	Type type;
	vector <Edge> to;
	GRBLinExpr in;
	GRBLinExpr out;
	int lv() const;
	int cc() const;
	int cc() { return concentration; };
	Node(int l = 0, int c = 0, Type t = ORD);
	void set(int l = 0, int c = 0, Type t = ORD);
	GRBVar& addEdge(Node& v, GRBModel* model);
	void printInfo() const;
	string name();
};

#endif //NODE_H
