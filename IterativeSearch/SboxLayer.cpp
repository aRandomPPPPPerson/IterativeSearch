#include "SboxLayer.h"

#include <assert.h>
#include <set>
using std::set;
#include <algorithm>
using std::sort;
using std::hex;
using std::dec;

int hammingWeight(int z) {
	int i = (int)z;
	i = (i & 0x55555555) + ((i >> 1) & 0x55555555);
	i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
	i = (i & 0x0F0F0F0F) + ((i >> 4) & 0x0F0F0F0F);
	i = (i * (0x01010101) >> 24);
	return i;
}

inline WordType Sbox::get_sbox(WordType input)const {
	if (input >= (1 << _input_bitnum)) throw "sbox out bounded";
	return _sbox[input];
}

inline int Sbox::get_ibn()const {
	return _input_bitnum;
}

inline int Sbox::get_obn()const {
	return _output_bitnum;
}

Sbox Sbox::inverse_sbox()const {
	if (_input_bitnum != _output_bitnum) throw "can't be inversed";
	vector<WordType> isbox(1 << _input_bitnum);
	for (int i = 0; i < (1 << _input_bitnum); i++)
		isbox[_sbox[i]] = i;
	return Sbox(isbox.begin(), _output_bitnum, _input_bitnum);
}

ostream& operator<<(ostream& out, const Sbox& obj) {
	out << "size:" << obj.get_ibn() << "*" << obj.get_obn() << "\n";
	for (int i = 0; i < (1 << obj.get_ibn()); i++) out << "0x" << hex << (int)obj.get_sbox(i) << " ";
	return out;
}

void DistributionTable::gen_wrt_attack_type(AttackType at) {
	switch (at)
	{
	case AttackType::DDT: gen_ddt();
		break;
	case AttackType::LAT: gen_lat();
		break;
	case AttackType::LAT2: gen_lat2();
		break;
	case AttackType::DLCT: gen_dlct();
		break;
	default:
		break;
	}
}

void DistributionTable::gen_ddt() {
	int input_size = (1 << get_ibn());
	int output_size = (1 << get_obn());
	for (int x = 0; x < input_size; x++)
		for (int d = 0; d < input_size; d++)
			_count_table[d][get_sbox(x) ^ get_sbox(x ^ d)]++;
	for (int x = 0; x < input_size; x++)
		for (int y = 0; y < output_size; y++) {
			if (x == 0 && y == 0) {
				_weight_table[x][y] = 0;
				_sign_table[x][y] = 1;
			}
			else if (_count_table[x][y] == 0) {
				_weight_table[x][y] = INF;
				_sign_table[x][y] = 0;
			}
			else {
				_weight_table[x][y] = (WeightType)get_ibn() - (WeightType)(log((WeightType)_count_table[x][y]) / log(2.0));
				_sign_table[x][y] = 1;
			}
		}
}

void DistributionTable::gen_lat() {
	int input_size = (1 << get_ibn());
	int output_size = (1 << get_obn());
	for (int in = 0; in < input_size; in++)
		for (int out = 0; out < output_size; out++)
			for (int x = 0; x < input_size; x++)
				if ((hammingWeight(x & in) + hammingWeight(get_sbox(x) & out)) % 2 == 0) _count_table[in][out]++;
	for (int in = 0; in < input_size; in++)
		for (int out = 0; out < output_size; out++) {
			if (_count_table[in][out] > input_size / 2) {
				_count_table[in][out] = _count_table[in][out] - input_size / 2;
				_sign_table[in][out] = 1;
			}
			else if (_count_table[in][out] < input_size / 2) {
				_count_table[in][out] = input_size / 2 - _count_table[in][out];
				_sign_table[in][out] = -1;
			}
			else {
				_count_table[in][out] = 0;
				_sign_table[in][out] = 0;
			}
		}
	for (int x = 0; x < input_size; x++)
		for (int y = 0; y < output_size; y++) {
			if (x == 0 && y == 0) {
				_weight_table[x][y] = (WeightType)get_ibn() - 1 - (WeightType)(log((WeightType)_count_table[x][y]) / log(2.0));
			}
			else if (_count_table[x][y] == 0) {
				_weight_table[x][y] = INF;
			}
			else {
				_weight_table[x][y] = (WeightType)get_ibn() - 1 - (WeightType)(log((WeightType)_count_table[x][y]) / log(2.0));
			}
		}
}

void DistributionTable::gen_lat2() {
	int input_size = (1 << get_ibn());
	int output_size = (1 << get_obn());
	for (int in = 0; in < input_size; in++)
		for (int out = 0; out < output_size; out++)
			for (int x = 0; x < input_size; x++)
				if ((hammingWeight(x & in) + hammingWeight(get_sbox(x) & out)) % 2 == 0) _count_table[in][out]++;
	for (int in = 0; in < input_size; in++)
		for (int out = 0; out < output_size; out++) {
			if (_count_table[in][out] > input_size / 2) {
				_count_table[in][out] = _count_table[in][out] - input_size / 2;
				_sign_table[in][out] = 1;
			}
			else if (_count_table[in][out] < input_size / 2) {
				_count_table[in][out] = input_size / 2 - _count_table[in][out];
				_sign_table[in][out] = 1;
			}
			else {
				_count_table[in][out] = 0;
				_sign_table[in][out] = 0;
			}
		}

	for (int x = 0; x < input_size; x++)
		for (int y = 0; y < output_size; y++) {
			if (x == 0 && y == 0) {
				_weight_table[x][y] = (WeightType)get_ibn() - 1 - (WeightType)(log((WeightType)_count_table[x][y]) / log(2.0));
				_weight_table[x][y] *= 2;
			}
			else if (_count_table[x][y] == 0) {
				_weight_table[x][y] = INF;
			}
			else {
				_weight_table[x][y] = (WeightType)get_ibn() - 1 - (WeightType)(log((WeightType)_count_table[x][y]) / log(2.0));
				_weight_table[x][y] *= 2;
			}
		}
}

void DistributionTable::gen_dlct() {
	int input_size = (1 << get_ibn());
	int output_size = (1 << get_obn());
	for (int delta = 0; delta < input_size; delta++) {
		for (int lambda = 0; lambda < output_size; lambda++) {
			for (int x = 0; x < input_size; x++) {
				if (hammingWeight(lambda & (get_sbox(x) ^ get_sbox(x ^ delta))) % 2 == 0) _count_table[delta][lambda]++;
			}
		}
	}
	for (int in = 0; in < input_size; in++)
		for (int out = 0; out < output_size; out++) {
			if (_count_table[in][out] > input_size / 2) {
				_count_table[in][out] = _count_table[in][out] - input_size / 2;
				_sign_table[in][out] = 1;
			}
			else if (_count_table[in][out] < input_size / 2) {
				_count_table[in][out] = input_size / 2 - _count_table[in][out];
				_sign_table[in][out] = -1;
			}
			else {
				_count_table[in][out] = 0;
				_sign_table[in][out] = 0;
			}
		}

	for (int x = 0; x < input_size; x++)
		for (int y = 0; y < output_size; y++) {
			if (x == 0 && y == 0) {
				_weight_table[x][y] = (WeightType)get_ibn() - 1 - (WeightType)(log((WeightType)_count_table[x][y]) / log(2.0));
			}
			else if (_count_table[x][y] == 0) {
				_weight_table[x][y] = INF;
			}
			else {
				_weight_table[x][y] = (WeightType)get_ibn() - 1 - (WeightType)(log((WeightType)_count_table[x][y]) / log(2.0));
;			}
		}
}

inline CostType DistributionTable::get_cost(WordType x, WordType y)const {
	if (x >= (1 << get_ibn()) || y >= (1 << get_obn())) throw "get cost out bounded";
	if (_weight_table[x][y] == INF) return CostType(INF, 0);
	if (_sign_table[x][y] == 1) return CostType(_weight_table[x][y], 0);
	if (_sign_table[x][y] == -1) return CostType(_weight_table[x][y], 1);
}

WeightType DistributionTable::get_weight(WordType x, WordType y)const {
	if (x >= (1 << get_ibn()) || y >= (1 << get_obn())) throw "get weight out bounded";
	return _weight_table[x][y]; 
}
int DistributionTable::get_sign(WordType x, WordType y)const { 
	if (x >= (1 << get_ibn()) || y >= (1 << get_obn())) throw "get sign out bounded";
	return _sign_table[x][y]; 
}

ostream& operator<<(ostream& out, const DistributionTable& obj) {
	int input_size = 1 << obj.get_ibn();
	int output_size = 1 << obj.get_obn();
	out << "分布表：" << "\n";
	out << dec;
	for (int i = 0; i < input_size; i++) { for (int j = 0; j < output_size; j++) out << obj._count_table[i][j] << " "; out << "\n"; }
	out << "重量表：" << "\n";
	out << dec;
	for (int i = 0; i < input_size; i++) {
		for (int j = 0; j < output_size; j++)
			if (obj._weight_table[i][j] != INF) out << obj._weight_table[i][j] << " ";
			else out << "- ";
		out << "\n";
	}
	out << "符号表：" << "\n";
	out << dec;
	for (int i = 0; i < input_size; i++) { for (int j = 0; j < output_size; j++) out << obj._sign_table[i][j] << " "; out << "\n"; }
	out << "概率表：" << "\n";
	for (int i = 0; i < input_size; i++) {
		for (int j = 0; j < output_size; j++)
			out << obj.get_cost(i, j) << "\t";
		out << "\n";
	}
	return out;
}

void AdvancedDistributionTable::gen() {
	int input_size = (1 << get_ibn());
	int output_size = (1 << get_obn());
	set<WeightType> w_set;
	for (int x = 0; x < input_size; x++)
		for (int y = 0; y < output_size; y++) {
			if (x == 0 && y == 0) continue;
			if(get_weight(x,y)!=INF)
				w_set.insert(get_weight(x,y));
		}
	_weight_vec.assign(w_set.begin(), w_set.end());

	for (int x = 0; x < input_size; x++) {
		vector<vector<WordType>> word_vec_vec;
		for (int k = 0; k < _weight_vec.size(); k++) {
			vector<WordType> word_vec;
			for (int y = 0; y < output_size; y++) {
				if (get_weight(x, y) == _weight_vec[k]) {
					word_vec.emplace_back(y);
				}
			}
			word_vec_vec.emplace_back(word_vec);
		}
		_ordered_output_table.emplace_back(word_vec_vec);
	}
	for (int x = 0; x < _ordered_output_table.size(); x++)
		for (int w = 0; w < _ordered_output_table[x].size(); w++)
			sort(_ordered_output_table[x][w].begin(), _ordered_output_table[x][w].end(), [](WordType a, WordType b) {return hammingWeight(a) < hammingWeight(b); });
	for (int x = 0; x < _ordered_output_table.size(); x++) {
		vector<vector<bool>> sign_vec_vec;
		for (int w = 0; w < _ordered_output_table[x].size(); w++) {
			vector<bool> sign_vec;
			for (int i = 0; i < _ordered_output_table[x][w].size(); i++) {
				sign_vec.emplace_back(get_sign(x, _ordered_output_table[x][w][i]) == 1 ? 0 : 1);
			}
			sign_vec_vec.emplace_back(sign_vec);
		}
		_ordered_sign_table.emplace_back(sign_vec_vec);
	}
}

ostream& operator<<(ostream& out, const AdvancedDistributionTable& obj) {
	int input_size = 1 << obj.get_ibn();
	int output_size = 1 << obj.get_obn();
	out << "排序的分布表：" << "\n";
	for (int x = 0; x < obj._ordered_output_table.size(); x++) {
		out << "x:" << hex << x << " ";
		for (int w = 0; w < obj._ordered_output_table[x].size(); w++) {
			out << dec << w << "(" << obj.get_output_size(x,w) << "): " << hex;
			for (int y = 0; y < obj._ordered_output_table[x][w].size(); y++)
				out << hex<<(int)obj.get_output_at(x, w, y) << "(" << obj.get_cost_at(x, w, y) << ") ";
			out << ";";
		}
		out << "\n";
	}
	out << "S盒分布表的weight：" << "\n";
	for (int i = 0; i < obj._weight_vec.size(); i++) out << obj._weight_vec[i] << " "; out << "\n";
	return out;
}


#include <iostream>
#include <fstream>
using std::cout;
using std::endl;
using std::ofstream;
void test_sbox_layer() {
	ofstream outfile;
	outfile.open("test_sbox_layer.txt");
	WordType* s = s_present;
	int input_bitnum = 4;
	int output_bitnum = 4;
	Sbox sbox(s, input_bitnum, output_bitnum);
	outfile << sbox << endl;
	Sbox isbox(sbox.inverse_sbox());
	outfile << isbox << endl;
	AdvancedDistributionTable DDT(s, input_bitnum, output_bitnum, AttackType::DDT);
	outfile << "DDT:" << endl;
	outfile << (DistributionTable)DDT << endl;
	outfile << DDT << endl;
	AdvancedDistributionTable LAT(s, input_bitnum, output_bitnum, AttackType::LAT);
	outfile << "LAT:" << endl;
	outfile << (DistributionTable)LAT << endl;
	outfile << LAT << endl;
	AdvancedDistributionTable LAT2(s, input_bitnum, output_bitnum, AttackType::LAT2);
	outfile << "LAT2:" << endl;
	outfile << (DistributionTable)LAT2 << endl;
	outfile << LAT2 << endl;
	DistributionTable DLCT(s, input_bitnum, output_bitnum, AttackType::DLCT);
	outfile << "DLCT:" << endl;
	outfile << DLCT << endl;
	outfile.close();
}