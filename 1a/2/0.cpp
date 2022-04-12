#include <iostream>
#include <string>
#include <string_view>

using namespace std;

std::string solve(const string& s) {
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
