#include <deque>
#include <iostream>

using namespace std;

namespace {

int solve(deque<int>& ds) {
	int res = 0;
	int mind = 0;

	while (!ds.empty()) {
		if (ds.front() < mind) {
			ds.pop_front();
			continue;
		}
		if (ds.back() < mind) {
			ds.pop_back();
			continue;
		}
		if (ds.front() < ds.back()) {
			mind = max(mind, ds.front());
			ds.pop_front();
			++res;
		} else {
			mind = max(mind, ds.back());
			ds.pop_back();
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
	deque<int> ds;
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
