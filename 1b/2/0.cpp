#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <queue>
#include <stack>
#include <unordered_map>
#include <vector>

using namespace std;

namespace {

vector<vector<int>> xs;
int64_t cur_min;

void iter(const int i, const int lastx, const int64_t cur_cost) {
	if (i >= xs.size()) {
		cur_min = min(cur_min, cur_cost);
		return;
	}
	do {
		int new_lastx = lastx;
		int64_t new_cost = cur_cost;
		for (auto x : xs[i]) {
			new_cost += abs(new_lastx - x);
			new_lastx = x;
		}
		if (new_cost < cur_min)
			iter(i + 1, new_lastx, new_cost);
	} while (next_permutation(xs[i].begin(), xs[i].end()));

}

int64_t solve() {
	cur_min = 0;
	int lastx = 0;
	for (int i = 0; i < xs.size(); ++i) {
		const auto min_xi = xs[i].front();
		const auto max_xi = xs[i].back();
		const auto med_xi = (min_xi + max_xi) / 2;
		if (lastx <= med_xi) {
			cur_min += abs(lastx - min_xi);
			lastx = max_xi;
		} else {
			cur_min += abs(lastx - max_xi);
			lastx = min_xi;
		}
		cur_min += abs(max_xi - min_xi);
	}

	iter(0, 0, 0);
	return cur_min;
}

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	for (int tn = 1; tn <= t; tn++) {
		size_t n, p;
		cin >> n >> p;
		xs.resize(n);
		for (size_t i = 0; i < n; ++i) {
			xs[i].resize(p);
			for (int j = 0; j < p; ++j) {
				int x;
				cin >> x;
				xs[i][j] = x;
			}
			sort(xs[i].begin(), xs[i].end());
		}
		cout << "Case #" << tn << ": " << solve() << endl;
	}

	return 0;
}
