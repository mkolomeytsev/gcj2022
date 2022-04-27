#include <algorithm>
#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <random>
#include <unordered_map>
#include <utility>
#include <vector>

using namespace std;

namespace {

using set256_t = bitset<256>;

uint8_t rotr(uint8_t b, uint8_t c) {
	return static_cast<uint8_t>(((bitset<8>(b) >> c) | (bitset<8>(b) << (8 - c))).to_ulong());
}

uint8_t calc_answer(const set256_t& possible_vs) {
	unordered_map<uint8_t, size_t> candy_ratio;
	unordered_map<uint8_t, set256_t> candy_power;

	for (size_t v_ = 0; v_ < 256; ++v_) {
		const auto v = static_cast<uint8_t>(v_);
		if (!possible_vs[v])
			continue;
		for (size_t candy_ = 0; candy_ < 256; ++candy_) {
			const auto candy = static_cast<uint8_t>(candy_);
			for (uint8_t r = 0; r < 8; ++r) {
				const uint8_t w = v ^ rotr(candy, r);
				if (!w)
					++candy_ratio[candy];
				candy_power[candy].set(w);
			}
		}
	}

	const auto candy_max_ratio = max_element(candy_ratio.begin(), candy_ratio.end(),
			[](const auto& kv1, const auto& kv2) { return kv1.second < kv2.second; })->second;
	vector<uint8_t> candy_max_ratio_all;
	for_each(candy_ratio.begin(), candy_ratio.end(),
			[&candy_max_ratio_all,candy_max_ratio](const auto& kv) { if (kv.second == candy_max_ratio) candy_max_ratio_all.push_back(kv.first); });

	const auto candy_min_power = *min_element(candy_max_ratio_all.begin(), candy_max_ratio_all.end(),
			[&candy_power](const auto& v1, const auto& v2) { return candy_power[v1].count() < candy_power[v2].count(); });
	vector<uint8_t> best_candies;
	copy_if(candy_max_ratio_all.begin(), candy_max_ratio_all.end(), back_inserter(best_candies),
			[&candy_power,candy_min_power](const auto v) { return candy_power[v].count() == candy_power[candy_min_power].count(); });

	random_device rand_dev;
	mt19937 generator(rand_dev());
	uniform_int_distribution<size_t> distr(0u, best_candies.size() - 1);
	const auto best_candy = best_candies[distr(generator)];
	return best_candy;
}

void update_possible_vs(set256_t* possible_vs, uint8_t my_answer, uint8_t their_answer) {
	set256_t new_possible_vs;
	for (size_t v_ = 0; v_ < 256; ++v_) {
		const auto v = static_cast<uint8_t>(v_);
		if (!(*possible_vs)[v])
			continue;
		for (uint8_t r = 0; r < 8; ++r) {
			const uint8_t w = v ^ rotr(my_answer, r);
			if (bitset<8>(w).count() == their_answer)
				new_possible_vs.set(w);
		}
	}
	*possible_vs = new_possible_vs;
}

void solve() {
	set256_t possible_vs = set256_t().flip();
	while (true) {
		const uint8_t my_answer = calc_answer(possible_vs);
		cout << bitset<8>(my_answer) << endl;

		int their_ans_;
		cin >> their_ans_;
		if (!their_ans_)
			break;
		if (their_ans_ < 0)
			quick_exit(0);
		const uint8_t their_answer = static_cast<uint8_t>(their_ans_);

		update_possible_vs(&possible_vs, my_answer, their_answer);
	}
}

}

int main() {
	int t;
	cin >> t;
	for (int tn = 1; tn <= t; tn++)
		solve();

	return 0;
}
