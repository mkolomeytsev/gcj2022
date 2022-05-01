#include <algorithm>
#include <vector>
#include <bitset>
#include <string>
#include <iostream>

using namespace std;

namespace {

string solve(vector<string>& ss) {
	for (size_t i = 0; i < ss.size(); ) {
		const char l_ch = ss[i].back();
		auto it = find_if(ss.begin(), ss.end(), [l_ch, as=&ss[i]](const auto& s) {
				return as != &s && all_of(s.begin(), s.end(), [l_ch](const char c) { return l_ch == c; } );
		});
		if (it != ss.end()) {
			ss[i] += *it;
			ss.erase(it);
			continue;
		}
		it = find_if(ss.begin(), ss.end(), [l_ch, as=&ss[i]](const auto& s) { return as != &s && l_ch == s.front(); });
		if (it != ss.end()) {
			ss[i] += *it;
			ss.erase(it);
			continue;
		}
		++i;		
	}
	string res;
	for (const auto& s : ss)
		res += s;
	char prev_ch = 0;
	bitset<32> ch_mask;
	for (const char c : res)
		if (prev_ch != c) {
			prev_ch = c;
			if (ch_mask[c - 'A'])
				return "IMPOSSIBLE";
			ch_mask.set(c - 'A');
		}
	return res;
		
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
