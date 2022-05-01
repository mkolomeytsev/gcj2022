#include <optional>
#include <functional>
#include <vector>
#include <bitset>
#include <string>
#include <iostream>

using namespace std;

namespace {

string solve(const vector<string>& ss) {
	vector<bitset<32>> wbcs;
	wbcs.reserve(ss.size());
	for (const auto& s : ss) {
		bitset<32> bcs;
		char prev = 0;
		for (const char c : s) {
			if (c != prev) {
				const auto ci = c - 'A';
				if (bcs[ci])
					return "IMPOSSIBLE";
				bcs.set(ci);
			}
			prev = c;
		}
		wbcs.push_back(bcs);
	}

	bitset<100> bws;
	bitset<32> bcs;
	optional<size_t> last;
	vector<size_t> res;
	function<bool()> iter = [&]() {
		bool has_free = false;
		for (size_t i = 0; i < ss.size(); ++i) {
			if (bws[i])
				continue;
			has_free = true;
			const auto& candy = ss[i];
			auto bcs_isect = bcs & wbcs[i];
			if (last && ss[*last].back() == candy.front())
				bcs_isect.reset(candy.front() - 'A');
			if (bcs_isect.any())
				continue;
			auto nbcs = bcs | wbcs[i];
			optional<size_t> saved_last = last;
			last = i;
			bws.set(i);
			swap(bcs, nbcs);
			if (iter()) {
				res.push_back(i);
				return true;
			}
			swap(bcs, nbcs);
			bws.reset(i);
			last = saved_last;
		}
		return !has_free;
	};
	if (!iter())
		return "IMPOSSIBLE";
	string s;
	for (const auto i : res)
		s = ss[i] + s;
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
