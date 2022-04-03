#include <iostream>
#include <string>
#include <string_view>

using namespace std;

void print_one(int r, int c) {
	static string maxdel;
	static string maxrow;

	const auto rowlen = 1+ (c<<1);

	if (maxdel.length() < rowlen) {
		maxdel = string(rowlen, '+');
		maxrow = string(rowlen, '|');
		for (int i = 3; i < rowlen; i += 2) {
			maxdel[i] = '-';
			maxrow[i] = '.';
		}
	}

	maxdel[0] = '.';
	maxdel[1] = '.';
	maxrow[0] = '.';
	maxrow[1] = '.';

	string_view del(maxdel);
	del.remove_suffix(del.length() - rowlen);
	cout << del << endl;

	string_view row(maxrow);
	row.remove_suffix(row.length() - rowlen);
	cout << row << endl;

	maxdel[0] = '+';
	maxdel[1] = '-';
	del = string_view(maxdel);
	del.remove_suffix(del.length() - rowlen);
	cout << del << endl;

	maxrow[0] = '|';
	maxrow[1] = '.';
	row = string_view(maxrow);
	row.remove_suffix(row.length() - rowlen);

	while (--r > 0) {
		cout << row << endl;
		cout << del << endl;
	}
}

int main() {

	int t;
	cin >> t;

	for (int tn = 1; tn <= t; tn++) {
		int r, c;
		cin >> r >> c;
		cout << "Case #" << tn << ":" << endl;
		print_one(r, c);
	}

	return 0;
}

