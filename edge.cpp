#include "edge.h"
#include "node.h"
#include <sstream>
using namespace std;

Edge::Edge(const Node* u, const Node* v) {
	start.lv = u->level;
	start.cc = u->concentration;
	start.type = u->type;
	end.lv = v->level;
	end.cc = v->concentration;
	end.type = v->type;
}

Edge::Edge() {
	start.lv = -1;
	start.cc = -1;
	start.type = ORD;
	end.lv = -1;
	end.cc = -1;
	end.type = ORD;
}

string Edge::name() const {
	string out;
	string num;
	stringstream ss;
	out += "L";
	ss << start.lv;
	ss >> num;
	out += num;
	out += "C";
	ss.clear();
	ss << start.cc;
	ss >> num;
	out += num;
	out += "toL";
	ss.clear();
	ss << end.lv;
	ss >> num;
	out += num;
	out += "C";
	ss.clear();
	ss << end.cc;
	ss >> num;
	out += num;
	return out;
}