#include <algorithm>
#include <cstdint>
#include <iostream>
#include <stack>
#include <tuple>
#include <vector>

using namespace std;

namespace {

using weight_t = uint8_t;
using number_t = uint8_t;
using numbers_t = vector<number_t>;
using numdiff_t = int;
using numdiffs_t = vector<numdiff_t>;
using cost_t = uint32_t;

vector<numbers_t> es;
vector<cost_t> es_min_est;
vector<numdiffs_t> diffs;

stack<weight_t> st;
numbers_t st_contents;
cost_t cur_min;

auto calc_diff(const numbers_t& s1, const numbers_t& s2, numdiffs_t& res) {
	cost_t neg_sum = 0;
	cost_t pos_sum = 0;
	const size_t sz = s1.size();
	for (size_t i = 0; i < sz; ++i) {
		const numdiff_t sd = s1[i] - s2[i];
		if (sd < 0)
			neg_sum -= sd;
		if (sd > 0)
			pos_sum += sd;
		res[i] = sd;
	}
	const bool equals = neg_sum == 0 && pos_sum == 0;
	return make_tuple(equals, neg_sum, pos_sum);
}

void iter(const cost_t step_no, size_t cur_e, const bool last_is_addition) {
	numdiffs_t& diff = diffs[step_no - 1];
	cost_t neg_sum;
	cost_t pos_sum;
	bool e_changed = false;
	do {
		bool equals;
		tie (equals, neg_sum, pos_sum) = calc_diff(es[cur_e], st_contents, diff);
		if (!equals)
			break;
		e_changed = true;
		if (++cur_e == es.size()) {
			const cost_t new_cost = step_no - 1 + st.size();
			cur_min = min(new_cost, cur_min);
			return;
		}
	} while (true);

	const auto low_est = step_no - 1 + pos_sum + st.size() + pos_sum + es_min_est[cur_e];
	if (low_est >= cur_min)
		return;

	if (!neg_sum) {
		for (weight_t i = 0; i < diff.size(); ++i) {
			if (diff[i] > 0) {
				st.push(i);
				++st_contents[i];
				iter(step_no + 1, cur_e, true);
				st.pop();
				--st_contents[i];
			}
		}
	}

	if (!st.empty() && (!last_is_addition || e_changed)) {
		const auto w_on_top = st.top();
		st.pop();
		--st_contents[w_on_top];
		iter(step_no + 1, cur_e, false);
		st.push(w_on_top);
		++st_contents[w_on_top];
	}
}

cost_t solve(const weight_t w) {
	st_contents.resize(w);
	es_min_est.resize(es.size());
	es_min_est.back() = 0;
	numdiffs_t a(w);
	for (size_t i = es.size() - 1; i > 0; --i) {
		auto [b, c, pos_sum] = calc_diff(es[i], es[i - 1], a);
		es_min_est[i - 1] = es_min_est[i] + pos_sum + pos_sum;	
	}

	cur_min = 0;
	for (const auto &u : es)
		for (const auto v : u)
			cur_min += v + v;

	diffs.resize(cur_min);
	for (auto& diff : diffs)
		diff.resize(w);

	iter(1, 0, false);

	return cur_min;
}

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	for (int tn = 1; tn <= t; tn++) {
		size_t e, w;
		cin >> e >> w;
		es.resize(e);
		for (size_t i = 0; i < e; ++i) {
			es[i].resize(w);
			for (weight_t j = 0; j < w; ++j) {
				size_t x;
				cin >> x;
				es[i][j] = x;
			}
		}
		cout << "Case #" << tn << ": " << solve(w) << endl;
	}

	return 0;
}
