#include <algorithm>
#include <iostream>
#include <set>
#include <vector>


using namespace std;

namespace {

auto solve(const size_t n, size_t k) {
	vector<vector<set<size_t>>> m(n + 2);
	for (auto& v : m)
		v.resize(n + 2);

	vector<vector<size_t>> o(n + 2);
	for (auto& v : o)
		v.resize(n + 2);

	const struct {
		int y;
		int x;
	} dyx[] = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

	size_t i = n / 2 + 1;
	size_t j = i;
	m[i][j].emplace(0);
	o[i][j] = n * n;
	size_t step = 3;
	for (size_t v = n * n - 1; v > 0; --v) {
		{
			size_t nstep = (step + 1) % 4;
		       	size_t ni = i + dyx[nstep].y;
		       	size_t nj = j + dyx[nstep].x;
			if (ni > 0 && nj > 0 && ni <= n && nj <= n && !o[ni][nj]) {
				i = ni;
				j = nj;
				step = nstep;
			} else {
			       	i += dyx[step].y;
			       	j += dyx[step].x;
			}
		}

		o[i][j] = v;
		auto& cur = m[i][j];

		for (const auto& d : dyx) {
			const auto ni = i + d.y;
			const auto nj = j + d.x;
			for (const auto p : m[ni][nj]) {
				if (p + 1 > k)
					continue;
				cur.emplace(p + 1);
			}
		}
	}

	vector<pair<size_t, size_t>> res;
	if (!m[1][1].count(k))
		return res;
	for (i = 1, j = 1; o[i][j] != n * n; --k) {
		const auto it = find_if(begin(dyx), end(dyx), [i, j, k, &m, &o](const auto& d) { return o[i + d.y][j + d.x] > o[i][j] && m[i + d.y][j + d.x].count(k - 1); });
		if (o[i][j] + 1 != o[i + it->y][j + it->x])
			res.emplace_back(o[i][j], o[i + it->y][j + it->x]);
		i += it->y;
		j += it->x;
	}

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

