#include <functional>
#include <vector>
#include <bitset>
#include <string>
#include <iostream>

using namespace std;

namespace {

string solve(const vector<string>& ss) {
	bitset<100> bws;
	bitset<32> bcs;
	vector<size_t > res;
	function<bool()> iter = [&]() {
		bool has_free = false;
		for (size_t i = 0; i < ss.size(); ++i) {
			if (bws[i])
				continue;
			const auto& candy = ss[i];
			has_free = true;
			auto nbcs = bcs;
			char prev = res.empty() ? 0 : ss[res.back()].back();
			bool ok = true;
			for (const char c : candy) {
				if (c != prev && prev)
					nbcs.set(prev - 'A');
				if (nbcs[c - 'A']) {
					ok = false;
					break;
				}
				prev = c;
			}
			if (ok) {
				res.push_back(i);
				bws.set(i);
				swap(bcs, nbcs);
				if (iter())
					return true;
				swap(bcs, nbcs);
				bws.reset(i);
				res.pop_back();
			}
		}
		return !has_free;
	};
	if (!iter())
		return "IMPOSSIBLE";
	string s;
	for (const auto i : res)
		s += ss[i];
	return s;
}

}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;
	vector<string> ss;
	for (int tn = 1; tn <= t; tn++) {
		size_t n;
		cin >> n;
		ss.resize(n);
		for (size_t i = 0; i < n; ++i)
			cin >> ss[i];
		cout << "Case #" << tn << ": " << solve(ss) << endl;
	}

	return 0;
}
