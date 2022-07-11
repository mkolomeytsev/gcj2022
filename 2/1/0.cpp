#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>
#include <vector>


using namespace std;

namespace {

auto solve(const size_t n, const size_t k) {
	const auto sh_cuts = [n]() {
		vector<vector<size_t>> m(n + 2);
		for (auto& v : m)
			v.resize(n + 2);
		const pair<int, int> dyx[] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
		size_t i = 1, j = 1, step = 0;
		m[i][j] = 1;
		for (size_t v = 2; v <= n * n; ++v) {
			do {
			       	size_t ni = i + dyx[step].first;
			       	size_t nj = j + dyx[step].second;
				if (ni > 0 && nj > 0 && ni <= n && nj <= n && !m[ni][nj]) {
					i = ni;
					j = nj;
					break;
				}
				step = (step + 1) % 4;
			} while (true);
			m[i][j] = v;
		}
		vector<size_t> res(n * n + 1);
		for (i = 1; i <= n; ++i)
			for (j = 1; j <= n; ++j) {
				const auto v = m[i][j];
				res[v] = max({m[i - 1][j], m[i + 1][j], m[i][j + 1], m[i][j - 1]});
				if (res[v] <= v + 1)
					res[v] = 0;
			}
		return res;
	}();

	vector<pair<size_t, size_t>> res;
	const auto n2 = n * n;
	function<bool(size_t, size_t)> iter = [n2, k, &sh_cuts, &iter, &res](const size_t v, const size_t step) {
		const auto k_r = step == k;
		const auto n2_r = v == n2;
		if (k_r || n2_r)
			return k_r && n2_r;
		const auto w = sh_cuts[v];
		if (w && iter(w, step + 1)) {
			res.push_back({v, sh_cuts[v]});
			return true;
		}
		if (iter(v + 1, step + 1))
			return true;
		return false;
	};

	iter(1, 0);
	reverse(res.begin(), res.end());
	return res;
}

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	for (int tn = 1; tn <= t; tn++) {
		size_t n, k;
		cin >> n >> k;
		cout << "Case #" << tn << ": ";
		if (const auto as = solve(n, k); !as.empty()) {
			cout << as.size() << endl;
			for (const auto& a : as)
				cout << a.first << ' ' << a.second << endl;
		} else {
			cout << "IMPOSSIBLE" << endl;
		}
	}

	return 0;
}
