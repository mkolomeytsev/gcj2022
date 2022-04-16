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

using weight_t = uint8_t;
using number_t = uint8_t;
using numbers_t = vector<number_t>;
using numdiff_t = int;
using numdiffs_t = vector<numdiff_t>;
using cost_t = uint32_t;

using stack_t = vector<weight_t>;

struct state_t {
	stack_t stack;
	uint8_t cur_e;
	cost_t cur_cost;
};

struct ediff_t {
	numdiffs_t diff;
	bool equals;
	cost_t neg_sum;
	cost_t pos_sum;
};

ediff_t calc_diff(const numbers_t& s1, const numbers_t& s2) {
	cost_t neg_sum = 0;
	cost_t pos_sum = 0;
	const size_t sz = s1.size();
	numdiffs_t res(sz);
	for (size_t i = 0; i < sz; ++i) {
		const numdiff_t sd = s1[i] - s2[i];
		if (sd < 0)
			neg_sum -= sd;
		if (sd > 0)
			pos_sum += sd;
		res[i] = sd;
	}
	const bool equals = neg_sum == 0 && pos_sum == 0;
	return {res, equals, neg_sum, pos_sum};
}

struct state_hash_t {
	size_t operator()(const state_t& s) const noexcept {
		size_t seed = s.cur_e;
		for (auto i : s.stack)
			seed ^= i + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		return seed;
	}
};

struct state_equal_t {
	size_t operator()(const state_t& s1, const state_t& s2) const noexcept {
		return s1.cur_e == s2.cur_e && s1.stack == s2.stack;
	}
};

cost_t solve(const vector<numbers_t>& es, const weight_t w) {
	cost_t cur_min = 0;
	cost_t last_e_size = 0;
	for (const auto& e : es) {
		last_e_size = accumulate(e.begin(), e.end(), 0u);
		cur_min += last_e_size + last_e_size;
	}

	vector<ediff_t> e_diffs;
	e_diffs.resize(es.size());
	e_diffs[0] = calc_diff(es[0], numbers_t(w));
	for (size_t i = 1; i < es.size(); ++i) {
		e_diffs[i] = calc_diff(es[i], es[i - 1]);
	}

	vector<cost_t> es_min_est;
        es_min_est.resize(es.size() + 1);
        es_min_est.back() = 0;
        for (size_t i = es.size(); i > 0; --i)
                es_min_est[i - 1] = es_min_est[i] + e_diffs[i - 1].pos_sum + e_diffs[i - 1].neg_sum;

	queue<state_t> sq;
	sq.push({{}, 0, 0});
	unordered_map<state_t, cost_t, state_hash_t, state_equal_t> bs;
	ediff_t ediff;
	while (!sq.empty()) {
		state_t& state = sq.front();
		stack_t& stack = state.stack;
		const uint8_t cur_e = state.cur_e;
		cost_t cur_cost = state.cur_cost;
		ediff = e_diffs[cur_e];
		do {
			cost_t new_cost = cur_cost + es_min_est[cur_e] + last_e_size;
			if (new_cost >= cur_min)
				break;
			if (bs[state] < cur_cost)
				break;
			while (ediff.neg_sum) {
				const weight_t top = stack.back();
				numdiff_t& top_diff = ediff.diff[top];
				if (top_diff < 0)
					--ediff.neg_sum;
				else
					++ediff.pos_sum;
				++top_diff;
				stack.pop_back();
				++cur_cost;
			}
			new_cost = cur_cost + ediff.pos_sum + es_min_est[cur_e + 1] + last_e_size;
			if (cur_e + 1 == es.size()) {
				cur_min = min(cur_min, new_cost);
				break;
			}
			if (new_cost >= cur_min)
				break;
			stack.reserve(stack.size() + ediff.pos_sum);
			auto stack_end = stack.end();
			for (weight_t i = 0; i < ediff.diff.size(); ++i) {
				while (ediff.diff[i]--)
					stack.push_back(i);
			}
			state.cur_e = static_cast<uint8_t>(cur_e + 1);
			state.cur_cost = cur_cost + ediff.pos_sum;
			do {
				do {
					auto [it, inserted] = bs.try_emplace(state, state.cur_cost);
					if (!inserted) {
						if (it->second < state.cur_cost)
							continue;
						else
							it->second = state.cur_cost;
					}
					sq.push(state);
				} while (next_permutation(stack_end, stack.end()));
				if (stack.begin() == stack_end)
					break;
				const weight_t top = *--stack_end;
				stack.erase(stack_end);
				stack.insert(upper_bound(stack_end, stack.end(), top), top);
				state.cur_cost += 2;
				new_cost += 2;
				if (new_cost >= cur_min)
					break;
			} while (true);
		} while (false);
		sq.pop();
	}

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
		vector<numbers_t> es(e);
		for (size_t i = 0; i < e; ++i) {
			es[i].resize(w);
			for (weight_t j = 0; j < w; ++j) {
				size_t x;
				cin >> x;
				es[i][j] = static_cast<number_t>(x);
			}
		}
		cout << "Case #" << tn << ": " << solve(es, static_cast<weight_t>(w)) << endl;
	}

	return 0;
}
