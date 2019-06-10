#include "randominput.h"
#include <cstdlib>
using namespace std;

SPInput random(int d, int N, Mode M) {
	SPInput rd;
	rd.d = d;
	if (M == SAMPLE) {
		rd.costs = 1;
		rd.costb = 0;
	}
	else if (M == WASTE) {
		rd.costs = 1;
		rd.costb = 1;
	}

	int mx = 1;
	for (int i = 1; i <= d; ++i) {
		mx *= 2;
	}
	mx += 1;
	vector<bool> occupied;
	for (int i = 0; i <= mx; ++i) {
		occupied.push_back(false);
	}

	for (int i = 0; i < N; ++i) {
		int tmp = rand() % mx;
		while (occupied[tmp]) {
			tmp = rand() % mx;
		}
		rd.TC.push_back(tmp);
		occupied[tmp] = true;
		rd.S.push_back(1);
	}

	return rd;
}