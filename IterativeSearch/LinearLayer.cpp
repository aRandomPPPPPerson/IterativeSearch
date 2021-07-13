#include "LinearLayer.h"
using std::hex;
using std::dec;
void LinearLayer::gen_p_table_lookup() {
	for (int x = 0; x < (1<<_sbox_output_bitnum); x++) {
		vector<State> vec;
		for (int i = 0; i < _sbox_num; i++) {
			vector<WordType> input_vec(_sbox_num, 0);
			input_vec[(size_t)_sbox_num - 1 - i] = x;
			vec.emplace_back(linear_transform(input_vec));
		}
		_p_table_lookup.emplace_back(vec);
	}
}

ostream& operator<<(ostream& out, const LinearLayer& obj) {
	for (int x = 0; x < (1 << obj._sbox_output_bitnum); x++) {
		for (int i = 0; i < obj._sbox_num; i++) {
			out << "[" << hex << x << "][" << dec << i << "]:\t" << obj._p_table_lookup[x][i] << "\n";
		}
	}
	return out;
}

State LinearLayer::p_lookup(const State& s)const {
	State s_out(s.get_sn());
	StateAi sai(s);
	for (int i = 0; i < sai.get_ai_size(); i++) {
		s_out ^= get_p_at(sai.get_word_ai(i), sai.get_ai(i));
	}
	return s_out;
}
