#include <cassert>
#include <cstdint>
#include <iostream>
#include <vector>


using namespace std;

namespace {

template<typename T>
T diff(const T a, const T b) {
        return a > b ? a - b : b - a;
}

struct mset {
	bool e : 1 = true;
	unsigned corner : 1 = false;
	unsigned : 6;
	unsigned step : 8 = 0;
	uint16_t l : 16 = 0;
	uint32_t r : 32 = 0;
	mset() = default;
	mset(uint16_t v) : e(false), l(v), r(v) {}
	bool empty() const {
		return e;
	}
	bool contains(size_t v) const {
		return !empty() && l <= v && v <= r && ~(l ^ v) & 1;
	}
	void add(const mset& b) {
		if (empty()) {
			*this = b;
			return;
		}
		if (b.empty())
			return;
		assert(l % 2 == b.l % 2);
		assert(r + 2 >= b.l && b.r + 2 >= l);
		if (l > b.l)
			l = b.l;
		if (r < b.r)
			r = b.r;
		assert(l % 2 == r % 2);
	}
	void inc() {
		assert(!empty());
		++l;
		++r;
	}
};
static_assert(sizeof(mset) == 8);

const uint16_t max_n = 9999;
const uint32_t max_n2 = max_n * max_n;
const uint16_t center_i = max_n / 2;
const uint16_t center_j = center_i;
const struct {
	int y;
	int x;
	unsigned z;
} dyx[] = {{0, -1, 2}, {1, 0, 0}, {0, 1, 6}, {-1, 0, 4}};

unsigned next_step(unsigned s) {
	return (s + 1u) & 3u;
}

auto next_yx(uint16_t i, uint16_t j, unsigned s) {
	return make_pair(static_cast<uint16_t>(i + dyx[s].y), static_cast<uint16_t>(j + dyx[s].x));
}

auto prev_yx(uint16_t i, uint16_t j, unsigned s) {
	return make_pair(static_cast<uint16_t>(i - dyx[s].y), static_cast<uint16_t>(j - dyx[s].x));
}

vector<vector<mset>> m(max_n);
uint16_t last_i = center_i;
uint16_t last_j = center_j;
uint32_t last_v = max_n2;

void fill(const uint16_t n) {
	const uint32_t n2 = n * n;
	auto& i = last_i;
	auto& j = last_j;
	unsigned step = 3;
	for (--last_v; last_v > max_n2 - n2; --last_v) {
		const auto pi = i;
		const auto pj = j;
		{
			const auto nstep = next_step(step);
			const auto [ni, nj] = next_yx(i, j, nstep);
			if (m[ni][nj].empty()) {
				i = ni;
				j = nj;
				step = nstep;
			} else {
			       	i += dyx[step].y;
			       	j += dyx[step].x;
			}
		}
		auto& cur = m[i][j];

		cur.add(m[pi][pj]);
		const auto nstep = next_step(step);
		const auto [ni, nj] = next_yx(i, j, nstep);
		if (m[ni][nj].empty()) {
			cur.corner = true;
		} else {
			cur.corner = false;
			cur.add(m[ni][nj]);
		}
		cur.inc();
		cur.step = step;
	}
	++last_v;
}

auto solve(const uint16_t n, const uint32_t k) {
	assert(n % 2 == 1);
	assert(k < n * n - 1);

	fill(n);

	uint16_t i = center_i - (n >> 1u);
	uint16_t j = i;
	vector<pair<uint32_t, uint32_t>> res;
	if (!m[i][j].contains(k))
		return res;
	uint32_t v = 1;
	for (auto pk = k - 1; i != center_i || j != center_j; --pk) {
		if (i == j && i < center_i) {
			const uint16_t n = ((center_i - i) << 1u) + 1u;
			const uint32_t n2 = n * n;
			const uint32_t r = ((n2 - pk) >> 3u) + 1u;
			const uint32_t nn = (r << 1u) + 1u;
			if (nn < n) {
				const uint32_t nn2 = nn * nn;
				v += n2 - nn2;
				pk -= n2 - nn2;
				j = i = center_i - r;
				assert(m[i][j].contains(pk + 1));
			}
		}

		const auto& cur = m[i][j];

		if (!cur.corner) {
			const auto ostep = next_step(cur.step);
			const auto [ni, nj] = next_yx(i, j, ostep);
			if (m[ni][nj].contains(pk)) {
				const auto r = max(diff(i, center_i), diff(j, center_j)) + 1u;
				const uint16_t n = (r << 1u) - 1u;
				const auto nv = v + (n << 2u) - 5u - dyx[ostep].z;
				i = ni;
				j = nj;		
				res.emplace_back(v, nv);
				v = nv;
				continue;
			}
		}

		const auto [pi, pj] = prev_yx(i, j, cur.step);
		assert(m[pi][pj].contains(pk));
		++v;
		i = pi;
		j = pj;
	}

	return res;
}

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	for (auto& v : m)
		v.resize(max_n);
	m[last_i][last_j] = mset(0);

	int t;
	cin >> t;
	for (int tn = 1; tn <= t; tn++) {
		uint16_t n;
	        uint32_t k;
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

