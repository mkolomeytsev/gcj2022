#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int calc_dice_num(vector<int>& ss) {
	sort(ss.begin(), ss.end());
	int k = 0;
	int i = 1;
	for (auto it = ss.cbegin(); it != ss.cend(); ++it, ++i) {
		if (*it >= i)
			++k;
	}
	return k;
}

int main() {
	vector<int> ss;

	int t;
	cin >> t;

	for (int tn = 1; tn <= t; tn++) {
		int n;
		cin >> n;

		ss.clear();
		ss.reserve(n);
		for (int nn = 0; nn < n; nn++) {
			int s;
			cin >> s;
			ss.push_back(s);
		}

		cout << "Case #" << tn << ": " << calc_dice_num(ss) << endl;
	}

	return 0;
}
