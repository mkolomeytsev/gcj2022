#include <iostream>
#include <string>
#include <string_view>

using namespace std;

std::string solve(const string& s) {
	string bs = s;
	int bi = 0;

	string ns;
	for (int i = 0; i < s.size(); ++i) {
		ns = s[i] + s.substr(i);
		string_view bv(bs);
		bv.remove_prefix(bi);
		if (string_view(ns) < bv) {
			bs.erase(bi);
			bs.append(ns);
			bi += 2;
		} else {
			++bi;
		}
	}

	return bs;
}

int main() {
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	int t;
	cin >> t;

	for (int tn = 1; tn <= t; tn++) {
		string s;
		cin >> s;
		cout << "Case #" << tn << ": " << solve(s) << endl;
	}

	return 0;
}
