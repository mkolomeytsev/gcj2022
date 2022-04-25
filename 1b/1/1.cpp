#include <iostream>
#include <vector>

using namespace std;

namespace {

int solve(const vector<int>& ds) {
	int res = 0;
	int mind = 0;

	for (int b = 0, e = ds.size() - 1; b <= e; ) {
		if (ds[b] < mind) {
			++b;
			continue;
		}
		if (ds[e] < mind) {
			--e;
			continue;
		}
		if (ds[b] < ds[e]) {
			mind = max(mind, ds[b]);
			++b;
			++res;
		} else {
			mind = max(mind, ds[e]);
			--e;
			++res;
		}
	}

	return res;
}

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	vector<int> ds;
	for (int tn = 1; tn <= t; tn++) {
		size_t n;
		cin >> n;
		ds.resize(n);
		for (size_t i = 0; i < n; ++i) {
			int d;
			cin >> d;
			ds[i] = d;
		}
		cout << "Case #" << tn << ": " << solve(ds) << endl;
	}

	return 0;
}
