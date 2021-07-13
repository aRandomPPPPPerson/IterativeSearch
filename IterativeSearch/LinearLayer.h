#pragma once
#include "State.h"
#include "linearInstances.h"
#include <functional>
using std::function;
#include <vector>
using std::vector;

class LinearLayer{
	function<vector<WordType> (const vector<WordType>&)> linear_transform;
	int _sbox_output_bitnum;
	int _sbox_input_bitnum;
	int _sbox_num;
	vector<vector<State>> _p_table_lookup;
	//TODO: add the input size is not equal to the output size. e.g. DES
	void gen_p_table_lookup();
public:
	LinearLayer(function<vector<WordType>(const vector<WordType>&)> f, int sob, int sib, int sn) :linear_transform(f), _sbox_output_bitnum(sob), _sbox_input_bitnum(sib), _sbox_num(sn) {
		gen_p_table_lookup();
	}
	int get_sobn()const { return _sbox_output_bitnum; }
	int get_sibn()const{return _sbox_input_bitnum;}
	int get_sn()const{return _sbox_num;}
	State get_p_at(WordType value, size_t index)const { return _p_table_lookup[value][index]; }
	State p_lookup(const State& s)const;
	friend ostream& operator<<(ostream& out, const LinearLayer& obj);
};
