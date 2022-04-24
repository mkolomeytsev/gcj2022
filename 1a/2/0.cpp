#include <algorithm>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <vector>
#include <unordered_set>

using namespace std;

namespace {

constexpr int max_num = 1000000000;

void solve(const int n) {
	vector<int> mys;
	mys.reserve(n);
	for (int my = 1; mys.size() < n && my <= max_num; my = my << 1)
		mys.push_back(my);

	unordered_set<int> mys_set(mys.begin(), mys.end());
	for (int my = 3; mys.size() < n; ++my) {
		if (!mys_set.count(my))
			mys.push_back(my);
	}

	for (auto my : mys)
		cout << my << ' ';
	cout << endl;

	vector<int> their;
	their.reserve(n);
	for (int i = 1; i <= n; ++i) {
		int b;
		cin >> b;
		their.push_back(b);
	}
	sort(their.begin(), their.end());

	vector<int> all;
	all.reserve(n << 1);
	copy(mys.begin(), mys.end(), back_inserter(all));
	copy(their.begin(), their.end(), back_inserter(all));

	vector<int> a, b;
	a.reserve(n);
	b.reserve(n);
	int64_t as = 0, bs = 0;
	while (!all.empty()) {
		int num = all.back();
		all.pop_back();
		if (as < bs) {
			a.push_back(num);
			as += num;
		} else {
			b.push_back(num);
			bs += num;
		}
	}
	for (auto num : a)
		cout << num << ' ';
	cout << endl;
}

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;

	for (int tn = 1; tn <= t; tn++) {
		int n;
		cin >> n;
		solve(n);
	}

	return 0;
}
