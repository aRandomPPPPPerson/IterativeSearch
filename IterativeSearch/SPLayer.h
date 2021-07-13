#pragma once

#include "SboxLayer.h"
#include "LinearLayer.h"
#include "State.h"
#include <ostream>
using std::ostream;

class SPLayer
:public AdvancedDistributionTable,public LinearLayer{
	vector<vector<vector<vector<State>>>> _sp_table_lookup;
	vector<vector<vector<vector<State>>>> _s_table_lookup;
	void gen_table_lookup();
public:
	template <class RandomAccessIterator>
	SPLayer(RandomAccessIterator first, int ibn, int obn, AttackType at, int sn, function<vector<WordType>(const vector<WordType>&)> f) :
		AdvancedDistributionTable(first,ibn,obn,at), LinearLayer(f,obn,ibn,sn)
		, _sp_table_lookup(1 << get_ibn(), vector<vector<vector<State>>>(get_sn(), vector<vector<State>>(get_weight_vec_size())))
		, _s_table_lookup(1 << get_ibn(), vector<vector<vector<State>>>(get_sn(), vector<vector<State>>(get_weight_vec_size())))
	{
		gen_table_lookup();
	}
	SPLayer(const Sbox& sbox, AttackType at, int sn, function<vector<WordType>(const vector<WordType>&)> f) :
		AdvancedDistributionTable(sbox, at), LinearLayer(f, sbox.get_obn(), sbox.get_ibn(), sn)
		, _sp_table_lookup(1 << get_ibn(), vector<vector<vector<State>>>(get_sn(), vector<vector<State>>(get_weight_vec_size())))
		, _s_table_lookup(1 << get_ibn(), vector<vector<vector<State>>>(get_sn(), vector<vector<State>>(get_weight_vec_size())))
	{
		gen_table_lookup();
	}
	size_t get_sp_size(int input, int index, int weight)const { return _sp_table_lookup[input][index][weight].size(); }
	State get_sp_at(int input, int index, int weight, int offset)const { return _sp_table_lookup[input][index][weight][offset]; }
	size_t get_s_size(int input, int index, int weight)const { return _s_table_lookup[input][index][weight].size(); }
	State get_s_at(int input, int index, int weight, int offset)const { return _s_table_lookup[input][index][weight][offset]; }
	friend ostream& operator<<(ostream& out, const SPLayer& obj);
};