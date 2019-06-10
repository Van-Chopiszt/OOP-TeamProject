#include "node.h"
#include <sstream>
using namespace std;

inline int Node::lv() const {
	return level;
}

inline int Node::cc() const {
	return concentration;
}

Node::Node(int l, int c, Type t) :type(t), level(l), concentration(c) {}

void Node::set(int l, int c, Type t) {
	type = t;
	level = l;
	concentration = c;
	to.resize(0);
}

GRBVar& Node::addEdge(Node& v, GRBModel* model) {
	to.push_back(Edge(this, &v));
	to.back().f = model->addVar(0.0, GRB_INFINITY, 0.0, GRB_INTEGER, to.back().name());
	out += to.back().f;
	v.in += to.back().f;
	return to.back().f;
}

void Node::printInfo() const {
	std::cout << type << ":" << level << " " << concentration << std::endl
		<< "Number of edges: " << to.size() << std::endl;
	int t = to.size();
	for (int i = 0; i < t; ++i) {
		std::cout << to[i].name() << std::endl;
		//std::cout << to[i].start.lv << " " << to[i].start.cc << " " << to[i].end.lv << " " << to[i].end.cc << std::endl;
	}

	int e = in.size();
	cout << e << endl;
	for (int i = 0; i < e; ++i) {
		std::cout << in.getCoeff(i) << " * " << in.getVar(i).get(GRB_StringAttr_VarName) << " + ";
	}
	std::cout << in.getConstant() << std::endl;

	int f = out.size();
	cout << f << endl;
	for (int i = 0; i < f; ++i) {
		std::cout << out.getCoeff(i) << " * " << out.getVar(i).get(GRB_StringAttr_VarName) << " + ";
	}
	std::cout << out.getConstant() << std::endl;
}

string Node::name() {
	stringstream ss;
	string name;
	string num;
	ss << level;
	ss >> num;
	ss.clear();
	name += num;
	name += ",";
	ss << concentration;
	ss >> num;
	name += num;
	return name;
}