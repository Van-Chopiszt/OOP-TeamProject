#include "Chip.h"
using namespace std;

Chip::Chip(int m_, int n_) :m(m_), n(n_) {
	map.push_back(vector<int>());
	for (int i = 1; i <= m; ++i) {
		vector<int> row;
		for (int j = 0; j <= n; ++j) {
			row.push_back(-1);
		}
		map.push_back(row);
	}
}

ostream& operator<<(ostream& os, const Chip& chip) {
	for (int i = 1; i <= chip.m; ++i) {
		for (int j = 1; j <= chip.n; ++j) {
			cout << chip.map[i][j] << '\t';
		}
		cout << endl;
	}
	cout << "Path 1: ";
	int sz1 = chip.route[0].size();
	for (int i = 0; i < sz1; ++i) {
		cout << "(" << chip.route[0][i].x << "," << chip.route[0][i].y << ")";
	}
	cout << endl;
	cout << "Path 2: ";
	int sz2 = chip.route[1].size();
	for (int i = 0; i < sz2; ++i) {
		cout << "(" << chip.route[1][i].x << "," << chip.route[1][i].y << ")";
	}
	cout << endl;
	if (chip.getB) {
		cout << "Buffer path: ";
		int sz3 = chip.BRoute.size();
		for (int i = 0; i < sz3; ++i) {
			cout << "(" << chip.BRoute[i].x << "," << chip.BRoute[i].y << ")";
		}
		cout << endl;
	}
	if (chip.getS) {
		cout << "Sample path: ";
		int sz3 = chip.SRoute.size();
		for (int i = 0; i < sz3; ++i) {
			cout << "(" << chip.SRoute[i].x << "," << chip.SRoute[i].y << ")";
		}
		cout << endl;
	}
	if (chip.discard) {
		cout << "Discard path: ";
		int sz3 = chip.discardRoute.size();
		for (int i = 0; i < sz3; ++i) {
			cout << "(" << chip.discardRoute[i].x << "," << chip.discardRoute[i].y << ")";
		}
		cout << endl;
	}
	return os;
}