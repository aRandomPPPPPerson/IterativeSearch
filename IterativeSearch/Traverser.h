#pragma once
#include "LooplessCode.h"
#include "PatternGenerator.h"
#include "SPLayer.h"
#include <memory>
using std::unique_ptr;

class InputTraverser {
	unique_ptr<ExchangeCode> active_ptr;
	unique_ptr<GrayCode> offset_ptr;
	State& state;
	bool _end_flag;
	bool _irs;
	int _an;
	int _sn;
	int _ibn;
	State first_state;
	vector<int> num;
	void init_offset();
	void next_offset();
	void init_active_and_init_offset();
	void next_active_and_init_offset();
public:
	InputTraverser(State& s, int an, int ibn, bool irs);
	void next();
	bool end()const { return _end_flag; }
	friend ostream& operator<<(ostream& out, const InputTraverser& obj);
};

class OutputTraverser {
	StateAi _input_state_ai;
	const SPLayer& _sp_table;
	bool _end_flag;
	bool _sign;
	WeightType _wlb;
	WeightType _wub;
	vector<int> gen_len();
	bool init_offset_n_state();
	bool init_weight();
	bool init_pattern();
	bool next_offset_n_state();
	bool next_weight();
	bool next_pattern();
public:
	State first_state;
	State& output_state;
	unique_ptr<PatternGeneratorFixingAn> pattern_ptr;
	unique_ptr<ExchangeGrayCode> weight_ptr;
	unique_ptr<GrayCode> offset_ptr;
	OutputTraverser(State& so, WeightType wlb, WeightType wub, const State& si, const SPLayer& sp);
	void next();
	bool end()const;
	CostType get_cost()const { return CostType(pattern_ptr->weight, _sign); }
	friend ostream& operator<<(ostream& out, const OutputTraverser& obj);
};

class OutputTraverser2 {
	StateAi _input_state_ai;
	const SPLayer& _sp_table;
	bool _end_flag;
	bool _sign;
	vector<int> gen_len();
	vector<int> pattern;
	bool init_offset_n_state();
	bool init_weight();
	bool next_offset_n_state();
	bool next_weight();
public:
	State first_state;
	State& output_state;
	unique_ptr<ExchangeGrayCode> weight_ptr;
	unique_ptr<GrayCode> offset_ptr;
	OutputTraverser2(State& so, const vector<int>& p, const State& si, const SPLayer& sp);
	void next();
	bool end()const;
	friend ostream& operator<<(ostream& out, const OutputTraverser& obj);
};