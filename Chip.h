#include <iostream>
#include <vector>
#include "Point.h"
using std::pair;
using std::vector;
typedef vector<Point> Path;
class Chip {
   public:
    vector<vector<int> > map;  //>=0:concentration -1:none
    int m, n, d;
    Path route[2], BRoute, SRoute, discardRoute;
    bool getB, getS, discard;
};
