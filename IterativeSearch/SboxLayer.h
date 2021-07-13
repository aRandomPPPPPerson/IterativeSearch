#pragma once

#include "State.h"
#include "CostType.h"
#include <vector>
using std::vector;
#include <ostream>
using std::ostream;
#include "SboxInstances.h"

enum class AttackType{
	DDT, LAT, LAT2, DLCT
};

class Sbox {
	int _input_bitnum;
	int _output_bitnum;
	vector<WordType> _sbox;
public:
	template <class RandomAccessIterator>
	Sbox(RandomAccessIterator first, int ibn, int obn) :_input_bitnum(ibn), _output_bitnum(obn), _sbox(first, first + (1 << ibn)) {}
	WordType get_sbox(WordType input)const;
	int get_ibn()const;
	int get_obn()const;
	Sbox inverse_sbox()const;
	friend ostream& operator<<(ostream& out, const Sbox& obj);
};

class DistributionTable
	:public Sbox{
	vector<vector<int>> _count_table;
	vector<vector<WeightType>> _weight_table;
	vector<vector<int>> _sign_table;
	void gen_ddt();
	void gen_lat();
	void gen_lat2();
	void gen_dlct();
	void gen_wrt_attack_type(AttackType at);
public:
	template <class RandomAccessIterator>
	DistributionTable(RandomAccessIterator first, int ibn, int obn, AttackType at) :Sbox(first,ibn,obn),
		_count_table(1<< get_obn(), vector<int>(1 << get_ibn(), 0)), _weight_table(1 << get_obn(), vector<WeightType>(1 << get_ibn(), 0)),
		_sign_table(1 << get_obn(), vector<int>(1 << get_ibn(), 0)) {
		gen_wrt_attack_type(at);
	}
	DistributionTable(const Sbox& sbox, AttackType at) :Sbox(sbox), _count_table(1 << get_obn(), vector<int>(1 << get_ibn(), 0)), _weight_table(1 << get_obn(), vector<WeightType>(1 << get_ibn(), 0)),
		_sign_table(1 << get_obn(), vector<int>(1 << get_ibn(), 0)) {
		gen_wrt_attack_type(at);
	}
	CostType get_cost(WordType x, WordType y)const;
	WeightType get_weight(WordType x, WordType y)const;
	int get_sign(WordType x, WordType y)const;
	friend ostream& operator<<(ostream& out, const DistributionTable& obj);
};

class AdvancedDistributionTable
	:public DistributionTable {
	vector<WeightType> _weight_vec;
	vector<vector<vector<WordType>>> _ordered_output_table;
	vector<vector<vector<bool>>> _ordered_sign_table;
	void gen();
public:
	template <class RandomAccessIterator>
	AdvancedDistributionTable(RandomAccessIterator first, int ibn, int obn, AttackType at) :DistributionTable(first, ibn, obn, at) { gen(); }
	AdvancedDistributionTable(const Sbox& sbox, AttackType at) :DistributionTable(sbox, at) { gen(); }
	size_t get_output_size(WordType input, size_t weight_index)const { return _ordered_output_table[input][weight_index].size(); }
	WordType get_output_at(WordType input, size_t weight_index, size_t offset)const { return _ordered_output_table[input][weight_index][offset]; }
	CostType get_cost_at(WordType input, size_t weight_index, size_t offset)const { return get_cost(input, get_output_at(input, weight_index, offset)); }
	bool get_sign_at(WordType input, size_t weight_index, size_t offset)const { return _ordered_sign_table[input][weight_index][offset]; }
	WeightType get_weight_at(WordType input, size_t weight_index, size_t offset)const{ return get_weight(input, get_output_at(input, weight_index, offset)); }
	size_t get_weight_vec_size()const { return _weight_vec.size(); }
	const vector<WeightType>& get_weight_vec()const { return _weight_vec; }
	WeightType get_weight_vec(size_t index)const { return _weight_vec[index]; }
	WeightType get_min_weight()const { return _weight_vec.front(); }
	WeightType get_max_weight()const { return _weight_vec.back(); }
	friend ostream& operator<<(ostream& out, const AdvancedDistributionTable& obj);
};

void test_sbox_layer();