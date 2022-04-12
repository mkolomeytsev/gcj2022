#include <algorithm>
#include <iostream>
#include <stack>
#include <tuple>
#include <vector>

using namespace std;

namespace {

vector<vector<int>> es;
vector<int> es_min_est;

stack<int> st;
vector<int> st_contents;
size_t cur_min;

tuple<vector<int>, bool, int, int> calc_ediff(const vector<int>& s1, const vector<int>& s2) {
	const auto sz = s1.size();
	vector<int> res(sz + 1);
	int neg_sum = 0;
	int pos_sum = 0;
	for (int i = 1; i < sz; ++i) {
		res[i] = s1[i] - s2[i];
		if (res[i] < 0)
			neg_sum -= res[i];
		if (res[i] > 0)
			pos_sum += res[i];
	}
	const bool equals = neg_sum == 0 && pos_sum == 0;
	return {std::move(res), equals, neg_sum, pos_sum};
}

void iter(int step_no, int cur_e) {
	vector<int> ediff;
	int neg_sum;
	int pos_sum;
	do {
		bool equals;
		tie (ediff, equals, neg_sum, pos_sum) = calc_ediff(es[cur_e], st_contents);
		if (!equals)
			break;

		if (++cur_e == es.size()) {
			cur_min = min(step_no - 1 + st.size(), cur_min);
			return;
		}
	} while (true);

	if (step_no - 1 + pos_sum + st.size() + pos_sum + es_min_est[cur_e] >= cur_min)
		return;

	if (!neg_sum) {
		for (int i = 1; i < ediff.size(); ++i) {
			if (ediff[i] > 0) {
				st.push(i);
				++st_contents[i];
				iter(step_no + 1, cur_e);
				st.pop();
				--st_contents[i];
			}
		}
	}

	if (!st.empty()) {
		int w_on_top = st.top();
		st.pop();
		--st_contents[w_on_top];
		iter(step_no + 1, cur_e);
		st.push(w_on_top);
		++st_contents[w_on_top];
	}
}

int solve(const int w) {
	st_contents.resize(w + 1);
	es_min_est.resize(es.size());
	es_min_est.back() = 0;
	for (int i = es.size() - 2; i >= 0; --i) {
		auto [a, b, c, pos_sum] = calc_ediff(es[i + 1], es[i]);
	       es_min_est[i] = es_min_est[i + 1] + pos_sum + pos_sum;	
	}

	cur_min = 0;
	for (const auto &u : es)
		for (const auto v : u)
			cur_min += v + v;
	iter(1, 0);

	return cur_min;
}

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	for (int tn = 1; tn <= t; tn++) {
		int e, w;
		cin >> e >> w;
		es.resize(e);
		for (int i = 0; i < e; ++i) {
			es[i].resize(w + 1);
			for (int j = 1; j <= w; ++j) {
				int x;
				cin >> x;
				es[i][j] = x;
			}
		}
		cout << "Case #" << tn << ": " << solve(w) << endl;
	}

	return 0;
}
