#include <cstdint>
#include <functional>
#include <vector>
#include <iostream>
#include <string>
#include <limits>

using namespace std;

namespace {

string solve(const vector<int>& es, size_t k) {
	vector<int64_t> res;
	function<bool(size_t, int64_t, int64_t)> iter = [&res,&iter](const size_t k, int64_t s1, int64_t s2) {
		if (!k || !s1)
			return false;
		int64_t root = s2 / 2 / s1 - s1 / 2;
		int64_t e_p = root, e_m = root - 1;
		int64_t prev_abs_p = numeric_limits<int64_t>::max();
		int64_t prev_abs_m = numeric_limits<int64_t>::max();
		bool finished;
		do {
			finished = true;
			if (e_p <= 1000000000000000000) {
				finished = false;
				const auto ns1 = s1 + e_p;
				const auto ns2 = s2 + e_p * e_p;
				const auto nabs = abs(ns2 - ns1 * ns1);
				if (!nabs) {
					res.push_back(e_p);
					return true;
				}
				if (k == 1) {
					if (nabs >= prev_abs_p)
						break;
					prev_abs_p = nabs;
				}
				if (iter(k - 1, ns1, ns2)) {
					res.push_back(e_p);
					return true;
				}
				++e_p;
			}
			if (e_m >= -1000000000000000000) {
				finished = false;
				const auto ns1 = s1 + e_m;
				const auto ns2 = s2 + e_m * e_m;
				const auto nabs = abs(ns2 - ns1 * ns1);
				if (!nabs) {
					res.push_back(e_m);
					return true;
				}
				if (k == 1) {
					if (nabs >= prev_abs_m)
						break;
					prev_abs_m = nabs;
				}
				if (iter(k - 1, ns1, ns2)) {
					res.push_back(e_m);
					return true;
				}
				--e_m;
			}
		} while (!finished);
		return false;
	};
	int64_t s1 = 0, s2 = 0;
	for (const auto e : es) {
		s1 += e;
		s2 += e * e;
	}
	if (s1 * s1 == s2)
		return "0 ";
	for (size_t r = 1; r <= k; ++r)
		if (iter(r, s1, s2)) {
			string s;
			for (const auto i : res)
				s = to_string(i) + ' ' + s;
			return s;
		}
	return "IMPOSSIBLE";
}

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	vector<int> es;
	for (int tn = 1; tn <= t; tn++) {
		size_t n, k;
		cin >> n >> k;
		es.resize(n);
		for (size_t i = 0; i < n; ++i)
			cin >> es[i];
		cout << "Case #" << tn << ": " << solve(es, k) << endl;
	}

	return 0;
}
