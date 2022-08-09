#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

namespace {

vector<uint64_t> sum;
vector<uint64_t> res;

auto solve(const uint64_t R) {
	if (sum.size() > R)
		return sum[R];
	const uint64_t r0 = sum.empty() ? 1 : sum.size();
	const auto r02 = r0 * r0;
	sum.resize(R + 1);
	res.resize(R + 1);

	const uint64_t x0 = sqrt((r02 - r0) / 2) + 1;
	for (auto x = x0, x2 = x * x; x <= R; x2 += x + x + 1, ++x) {
		const auto x2mx = x2 - x;
		const auto x2px = x2 + x;
		uint64_t y = r02 < r0 + x2 + 1 ? 1 : sqrt(r02 - r0 - x2) + 1;
		uint64_t r = sqrt(x2 + y * y);
		auto r2 = r * r;
		auto r2r = r2 + r;
		auto rnext2 = r2r + r + 1;
		for (; y <= x; ++y) {
			const auto y2 = y * y;
			const auto x2y2 = x2 + y2;
			if (x2y2 >= rnext2) {
				++r;
				r2 = rnext2;
				r2r = r2 + r;
				rnext2 = r2r + r + 1;
				if (r > R) [[unlikely]]
					break;
			}
			uint64_t lr;
			uint64_t lr2;
			if (x2y2 > r2r) {
				lr = r + 1;
				lr2 = rnext2;
				if (lr > R) [[unlikely]]
					break;
			} else {
				lr = r;
				lr2 = r2;
			}
			const auto xx2 = lr2 - y2;
			if (xx2 <= x2mx || xx2 > x2px) {
				auto& s = res[lr];
				s += 2;
				if (x == y) [[unlikely]]
					--s;
			}
		}
	}

	for (auto i = r0; i <= R; ++i)
		sum[i] = sum[i - 1] + (res[i] << 2);
	return sum[R];
}

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	for (int tn = 1; tn <= t; tn++) {
		uint64_t R;
		cin >> R;
		cout << "Case #" << tn << ": " << solve(R) << endl;
	}

	return 0;
}

