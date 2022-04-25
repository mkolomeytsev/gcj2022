#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>
#include <utility>

using namespace std;

namespace {

template<typename T>
T diff(const T a, const T b) {
	return a > b ? a - b : b - a;
}

using num_t = size_t;
using cost_t = uint64_t;
vector<pair<num_t,num_t>> xs;
cost_t cur_min;

void iter(const size_t i, const num_t lastx, const cost_t cur_cost) {
	if (i >= xs.size()) {
		cur_min = min(cur_min, cur_cost);
		return;
	}

	auto& xi = xs[i];
	for (int k = 0; k < 2; ++k) {
		const cost_t new_cost = cur_cost + diff(xi.first, lastx) + diff(xi.second, xi.first);
		if (new_cost < cur_min)
			iter(i + 1, xi.second, new_cost);
		if (xi.first == xi.second)
			break;
		swap(xi.first, xi.second);
	}
}

cost_t solve() {
	cur_min = 0;
	num_t lastx = 0;
	for (const auto& xi : xs) {
		const auto med_xi = (xi.first + xi.second) / 2;
		if (lastx < med_xi) {
			cur_min += diff(lastx, xi.first);
			lastx = xi.second;
		} else {
			cur_min += diff(lastx, xi.second);
			lastx = xi.first;
		}
		cur_min += xi.second - xi.first;
	}

	iter(0, 0, 0);
	return cur_min;
}

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	size_t t;
	cin >> t;
	vector<num_t> tmp;
	for (size_t tn = 1; tn <= t; tn++) {
		size_t n, p;
		cin >> n >> p;
		xs.resize(n);
		tmp.resize(p);
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < p; ++j)
				cin >> tmp[j];
			auto [mit, mat] = minmax_element(tmp.begin(), tmp.end());
			xs[i] = { *mit, *mat };
		}
		cout << "Case #" << tn << ": " << solve() << endl;
	}

	return 0;
}
