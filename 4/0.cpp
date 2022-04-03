#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <utility>
#include <vector>

using namespace std;

struct node {
	int f;
	vector<node*> prevs;
	node(int f) : f(f) {}
};

using subtree_fun_t = pair<int, int64_t>;

subtree_fun_t calc_subtree_fun(const node& head) {
	if (head.prevs.empty())
		return subtree_fun_t(head.f, 0);
	int64_t sum = 0;
	int new_min = numeric_limits<int>::max();
	for (const node* prev : head.prevs) {
		auto [chain_min, other_sum] = calc_subtree_fun(*prev);
		sum += other_sum;
		if (new_min != numeric_limits<int>::max()) {
			if (new_min > chain_min) {
				sum += new_min;
				new_min = chain_min;
			} else {
				sum += chain_min;
			}
		} else {
			new_min = chain_min;
		}
	}

	return subtree_fun_t(max(head.f, new_min), sum);
}

int calc_fun(const vector<node*>& ends) {
	int64_t sum = 0;
	for (const node* end : ends) {
		auto [chain_min, other_sum] = calc_subtree_fun(*end);
		sum += chain_min;
		sum += other_sum;
	}
	return sum;
}


int main() {
	vector<node> nodes;
	vector<node*> ends;

	int t;
	cin >> t;

	for (int tn = 1; tn <= t; tn++) {
		int n;
		cin >> n;

		nodes.clear();
		nodes.reserve(n);
		for (int i = 0; i < n; ++i) {
			int f;
			cin >> f;
			nodes.emplace_back(f);
		}

		ends.clear();
		for (int i = 0; i < n; ++i) {
			int p;
			cin >> p;
			if (p != 0)
				nodes[p - 1].prevs.push_back(&nodes[i]);
			else
				ends.push_back(&nodes[i]);
		}

		cout << "Case #" << tn << ": " << calc_fun(ends) << endl;
	}

	return 0;
}

