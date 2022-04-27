#include <array>
#include <bitset>
#include <cstdint>
#include <functional>
#include <iostream>
#include <iterator>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

namespace {

/*
struct step_t {
	step_t* prev_;
	bitset<8> r;
	step_t* prev() const { (prev_ & 1) ? nullptr_t : prev_; }
	size_t startId() const { (prev_ & 1) ? (prev_>>1) : -1; }
};
*/
using set256_t = bitset<256>;
using set8_t = bitset<8>;

uint8_t rotr(uint8_t b, uint8_t c) {
	return static_cast<uint8_t>(((bitset<8>(b) >> c) | (bitset<8>(b) << (8 - c))).to_ulong());
}

struct pair_hash
{
	template <class T1, class T2>
	size_t operator() (const pair<T1, T2>& pair) const {
		return hash<T1>()(pair.first) ^ hash<T2>()(pair.second);
	}
};

void solve() {
	set256_t starts = set256_t().flip();
//	vector<set8_t> rotates;
	vector<uint8_t> there_answers;
	vector<uint8_t> my_answers;

	const auto calc_answer = [&]() {
//		unordered_set<pair<size_t, uint8_t>, pair_hash> active_rotates;
		unordered_map<uint8_t, size_t> possible_my_ans_freq;
		unordered_map<uint8_t, set256_t> gg;

		set256_t new_starts;
		uint64_t iter_cnt = 0;
		const function<bool(size_t, uint8_t)> iter = [&](const size_t step, const uint8_t v) {
			++iter_cnt;
			if (step == my_answers.size()) {
				new_starts.set(v);
				for (size_t my_ans_ = 0; my_ans_ < 256; ++my_ans_) {
					const auto possible_my_ans = static_cast<uint8_t>(my_ans_);
					for (uint8_t r = 0; r < 8; ++r) {
						const uint8_t w = v ^ rotr(possible_my_ans, r);
						if (!w)
							++possible_my_ans_freq[possible_my_ans];
						gg[possible_my_ans].set(w);
					}
				}
				return true;
			}
			bool found = false;
			for (uint8_t r = 0; r < 8; ++r) {
//				if (!rotates[step][r])
//					continue;
				const uint8_t w = v ^ rotr(my_answers[step], r);
				if (bitset<8>(w).count() != there_answers[step])
					continue;
				if (iter(step + 1, w)) {
					found = true;
//					active_rotates.insert({step, r});
				}
			}
			return found;
		};


		for (size_t start = 0; start < 256; ++start) {
			if (!starts[start])
				continue;
			if (!iter(0, static_cast<uint8_t>(start))) {
//				cerr << "Minus start: " << start << endl;
				starts.reset(start);
			}
		}

		/*
		for (size_t step = 0; step < rotates.size(); ++step) {
			for (size_t r = 0; r < 8; ++r) {
				if (!rotates[step][r])
					continue;
				if (!active_rotates.count({step, r})) {
					rotates[step].reset(r);
					cerr << "Minus rotate: " << step << " " << r << endl;
				}
			}
		}
		*/

		pair<uint8_t, size_t> max_possible = *max_element(possible_my_ans_freq.begin(), possible_my_ans_freq.end(), [](const auto& kv1, const auto& kv2) { return kv1.second < kv2.second; });
		vector<pair<uint8_t, size_t>> equal_best;
		copy_if(possible_my_ans_freq.begin(), possible_my_ans_freq.end(), back_inserter(equal_best), [&max_possible](const auto& kv) { return kv.second == max_possible.second; });
		pair<uint8_t, size_t> mp = *min_element(equal_best.begin(), equal_best.end(), [&gg](const auto& kv1, const auto& kv2) { return gg[kv1.first].count() < gg[kv2.first].count(); });
		vector<pair<uint8_t, size_t>> eb;
		copy_if(equal_best.begin(), equal_best.end(), back_inserter(eb), [&mp,&gg](const auto& kv) { return gg[kv.first].count() == gg[mp.first].count(); });

		pair<uint8_t, set256_t> hh = *min_element(gg.begin(), gg.end(), [](const auto& kv1, const auto& kv2) { return kv1.second.count() < kv2.second.count(); });
		vector<pair<uint8_t, set256_t>> equal_best_gg;
		copy_if(gg.begin(), gg.end(), back_inserter(equal_best_gg), [&hh](const auto& kv) { return kv.second.count() == hh.second.count(); });

/*
		for (const auto& v : equal_best)
			cerr << (int)v.first << ' ' << v.second << ", ";
		cerr << endl;
		for (const auto& v : gg)
			cerr << (int)v.first << ' ' << v.second.count() << ", ";
		cerr << endl;
		for (const auto& v : eb)
			cerr << (int)v.first << ", ";
		cerr << endl;
*/

		random_device rand_dev;
		mt19937 generator(rand_dev());
		uniform_int_distribution<size_t> distr(0u, equal_best.size() - 1);
		uniform_int_distribution<size_t> distr2(0u, equal_best_gg.size() - 1);
		uniform_int_distribution<size_t> distr3(0u, eb.size() - 1);
		max_possible = equal_best[distr(generator)];
		hh = equal_best_gg[distr2(generator)];
		mp = eb[distr3(generator)];

/*
		cerr << "Iters: " << iter_cnt << endl;
		cerr << "Starts: " << starts.count() << endl;
		cerr << "Answer freq: " << mp.second << endl;
		cerr << "\t(identical: " << equal_best.size() << " of " << possible_my_ans_freq.size() << ")" << endl;
		cerr << "\t(identical: " << equal_best_gg.size() << " of " << gg.size() << ")" << endl;
		cerr << "\t(identical: " << eb.size() << ")" << endl;
*/

		starts = new_starts;
		there_answers.clear();
		my_answers.clear();

		return mp.first;
//		return max_possible.first;
//		return hh.first;
	};

	while (true) {
		const auto my_ans = calc_answer();
		cout << bitset<8>(my_ans) << endl;
		my_answers.push_back(my_ans);

		int there_ans;
		cin >> there_ans;
		if (!there_ans)
			break;
		if (there_ans < 0)
			quick_exit(0);
		there_answers.push_back(static_cast<uint8_t>(there_ans));
//		rotates.push_back(set8_t().flip());
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
