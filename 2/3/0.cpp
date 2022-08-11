#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <vector>

using namespace std;

struct coord_t {
	int32_t x;
	int32_t y;
};

namespace {

auto solve(const vector<coord_t>& A, const vector<coord_t>& B) {
	vector<vector<int64_t>> A2B(A.size());
	for (size_t i = 0; i < A.size(); ++i) {
		auto& aib = A2B[i];
		aib.resize(B.size());
		for (size_t j = 0; j < B.size(); ++j) {
			const int64_t dx = B[j].x - A[i].x;
			const int64_t dy = B[j].y - A[i].y;
			aib[j] = dx * dx + dy * dy;
		}
	}
	vector<vector<int64_t>> B2A(B.size());
	for (size_t i = 0; i < B.size(); ++i) {
		B2A[i].resize(A.size());
		for (size_t j = 0; j < A.size(); ++j)
			B2A[i][j] = A2B[j][i];
	}

	vector<bool> af(A.size());
	vector<bool> bf(B.size());
	vector<pair<uint16_t, uint16_t>> res;

	function<bool()> iter = [&iter, &A2B, &af, &bf, &res]() {
		vector<pair<size_t, size_t>> cnt;
		for (size_t i = 0; i < A2B.size(); ++i) {
			if (af[i])
				continue;
			cnt.emplace_back(i, 0u);
			for (size_t j = 1; j < A2B[i].size(); ++j)
				if (!bf[j])
					cnt.back().second += A2B[i][j] <= A2B[i][0];
		}
		if (cnt.empty())
			return true;
		sort(begin(cnt), end(cnt), [](const auto& a, const auto& b) { return a.second < b.second; });
		for (const auto& c : cnt) {
			if (!c.second)
				return false;
			vector<pair<size_t, int64_t>> m;
			for (size_t j = 1; j < A2B[c.first].size(); ++j)
				if (!bf[j])
					m.emplace_back(j, A2B[c.first][j]);
			sort(begin(m), end(m), [](const auto& a, const auto& b) { return a.second < b.second; });
			for (auto it = begin(m); it != end(m) && it->second == m.front().second; ++it) {
				af[c.first] = true;
				bf[it->first] = true;
				if (iter()) {
					res.emplace_back(c.first, it->first);
					return true;
				}
				af[c.first] = false;
				bf[it->first] = false;
			}
		}
		return false;
	};

	iter();

	reverse(begin(res), end(res));

	return res;
}

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	for (int tn = 1; tn <= t; tn++) {
		uint16_t N;
		cin >> N;

		vector<coord_t> A(N);
		for (uint16_t i = 0; i < N; ++i)
			cin >> A[i].x >> A[i].y;

		vector<coord_t> B(N + 1);
		for (uint16_t i = 0; i <= N; ++i)
			cin >> B[i].x >> B[i].y;

		cout << "Case #" << tn << ": ";
		if (const auto res = solve(A, B); res.empty()) {
			cout << "IMPOSSIBLE" << endl;
		} else {
			cout << "POSSIBLE" << endl;
			for (size_t i = 0; i < res.size(); ++i)
				cout << res[i].first + 1 << ' ' << res[i].second + 1 << endl;
		}
	}

	return 0;
}
