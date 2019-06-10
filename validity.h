#ifndef VALIDITY_H
#define VALIDITY_H

#include "settings.h"
#include "stepinfo.h"
#include <vector>
#include <iostream>
using std::vector;
using std::ostream;

struct SBWD {
	int sample;
	int buffer;
	int waste;
	int dilution;
};

class SBWDResult {
	vector<SBWD> resarr;
	int size;
public:
	SBWDResult(): size(0) {};
	void append(const SBWD& res);
	double ave_S();
	double ave_B();
	double ave_W();
	double ave_D();	
	friend ostream& operator<<(ostream& os, SBWDResult& R);
};

class DropletTracker {
	int* dr;	
	int d;
	int oprt; //number of operations;
	int onboard;
public:
	int* p2;
	DropletTracker(int d_);
	DropletTracker(const SPInput& I);
	bool match(const SPInput& I);
	~DropletTracker();
	void process(const Node& node);
	void process(Node** NF, const Node& Source, vector<StepInfo>& cmd);
	int operator[](int i) { return dr[i]; };
	int getopr()const { return oprt; };
	int getonb()const { return onboard; };
	void print();
};

bool check(const SPInput& I, Node** NF, const Node& Source, SBWD& result);

void print(const SBWD& a);

#endif //VALIDITY_H
