#pragma once
#include "CostType.h"
#include <vector>
using std::vector;
#include <ostream>
using std::ostream;

class PatternGenerator {
	vector<WeightType> _weight_vec;
	WeightType _wlb;
	WeightType _wub;
	bool _backtrack;
	bool _increment;
	bool _end_flag;
public:
	vector<int> num;
	WeightType weight;
	int an;
	PatternGenerator(const vector<WeightType>& wv, WeightType wlb, WeightType wub);
	void next();
	bool end()const { return _end_flag; }
	friend ostream& operator<<(ostream& out, const PatternGenerator& obj);
};

class PatternGeneratorFixingAn {
	vector<WeightType> _weight_vec;
	WeightType _wlb;
	WeightType _wub;
	bool _backtrack;
	bool _end_flag;
	int _fan;
public:
	vector<int> num;
	WeightType weight;
	int an;
	PatternGeneratorFixingAn(const vector<WeightType>& wv, WeightType wlb, WeightType wub, int fan);
	void next();
	bool end()const { return _end_flag; }
	friend ostream& operator<<(ostream& out, const PatternGeneratorFixingAn& obj);
};

