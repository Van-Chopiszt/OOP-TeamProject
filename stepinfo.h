#ifndef STEPINFO_H
#define STEPINFO_H

struct StepInfo {
	bool buffer, sample;
	int merge1, merge2;
	int waste;
	StepInfo() :buffer(0), sample(0), merge1(-1), merge2(-1), waste(-1) {};
};

#endif //STEPINFO_H
