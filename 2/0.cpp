#include <algorithm>
#include <iostream>
#include <optional>

using namespace std;

struct cmyk {
	int c, m, y, k;
};

const int max_units = 1000000;

optional<cmyk> calc_color(cmyk ps[3]) {
	int units = max_units;

	cmyk res;

	res.c = std::min({ps[0].c, ps[1].c, ps[2].c, units});
	units -= res.c;

	res.m = std::min({ps[0].m, ps[1].m, ps[2].m, units});
	units -= res.m;

	res.y = std::min({ps[0].y, ps[1].y, ps[2].y, units});
	units -= res.y;

	res.k = std::min({ps[0].k, ps[1].k, ps[2].k, units});
	units -= res.k;

	if (units != 0)
		return {};

	return res;
}

int main() {
	int t;
	cin >> t;

	for (int tn = 1; tn <= t; tn++) {
		cmyk ps[3];
		for (int i = 0; i < 3; i++) {
			cmyk& p = ps[i];
			cin >> p.c >> p.m >> p.y >> p.k;
		}
		const auto ret = calc_color(ps);
		cout << "Case #" << tn << ": ";
		if (ret) {
			const cmyk res = *ret;
			cout << res.c << ' ' << res.m << ' ' << res.y << ' ' << res.k << endl;
		} else {
			cout << "IMPOSSIBLE" << endl;
		}
	}

	return 0;
}
