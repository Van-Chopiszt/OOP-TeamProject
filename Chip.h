#ifndef CHIP_H
#define CHIP_H

#include <iostream>
#include <vector>
#include "Point.h"
using std::ostream;
using std::pair;
using std::vector;
typedef vector<Point> Path;
class Chip {
private:
	int m, n;
public:
    vector<vector<int> > map;  //>=0:concentration -1:none
    Path route[2], BRoute, SRoute, discardRoute;
    bool getB, getS, discard;
	Chip(int m_, int n_);
	friend ostream& operator<<(ostream& os, const Chip& chip);
};

#endif //CHIP_H
