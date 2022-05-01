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
		if (!k)
			return false;
		int64_t prev_abs = numeric_limits<int64_t>::max();
		for (int64_t e = 0; e <= 1000000000000000000; ++e) {
			const auto ns1 = s1 + e;
			const auto ns2 = s2 + e*e;
			const auto nabs = abs(ns2 - ns1*ns1);
			if (!nabs) {
				res.push_back(e);
				return true;
			}
			if (k == 1 && nabs >= prev_abs)
				break;
			prev_abs = nabs;
			if (iter(k - 1, ns1, ns2)) {
				res.push_back(e);
				return true;
			}
		}
		prev_abs = numeric_limits<int64_t>::max();
		for (int64_t e = 1; e <= 1000000000000000000; ++e) {
			const auto ns1 = s1 - e;
			const auto ns2 = s2 + e*e;
			const auto nabs = abs(ns2 - ns1*ns1);
			if (!nabs) {
				res.push_back(-e);
				return true;
			}
			if (k == 1 && nabs >= prev_abs)
				break;
			prev_abs = nabs;
			if (iter(k - 1, ns1, ns2)) {
				res.push_back(e);
				return true;
			}
		}
		return false;
	};
	int64_t s1 = 0, s2 = 0;
	for (const auto e : es) {
		s1 += e;
		s2 += e * e;
	}
	if (!iter(k, s1, s2))
		return "IMPOSSIBLE";
	string s;
	for (const auto i : res)
		s = to_string(i) + ' ' + s;
	return s;
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
