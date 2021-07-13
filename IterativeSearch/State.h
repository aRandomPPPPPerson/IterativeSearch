#pragma once
#include "Types.h"

#include <emmintrin.h>
#include <vector>
using std::vector;
#include <ostream>
using std::ostream;
#include <map>
using std::map;

/*typedef union data128 {
	__m128i word128;
	uint8_t __declspec(align(16)) word8[16];
	uint64_t word64[2];
}u128;*/

class State {
	int _u8_num;
	vector<__m128i> _u128_vec;
public:
	State(int sn);
	State(const vector<WordType>& s);

	State& operator=(const State& rhs);
	const State operator^(const State& rhs)const;
	State& operator^=(const State& rhs);
	const State operator&(const State& rhs)const;
	State& operator&=(const State& rhs);
	State operator<<(int n)const;
	State operator>>(int n)const;

	bool operator>(const State& rhs)const;
	bool operator<(const State& rhs)const;
	bool operator==(const State& rhs)const;
	bool operator>=(const State& rhs)const;
	bool operator<=(const State& rhs)const;
	
	void set(size_t pos, WordType val);
	void set0();

	WordType get_word(size_t pos)const;
	int get_sn()const;
	int get_an()const;
	int hw()const;
	vector<int> get_ai()const;

	friend std::hash<State>;
	friend ostream& operator<<(ostream& out, const State& obj);
};

class StateAi
	:public State {
	vector<int> indice;
public:
	StateAi(const State& s) :State(s) { indice = s.get_ai(); }
	WordType get_word_ai(size_t pos)const;
	int get_ai(size_t pos)const;
	size_t get_ai_size()const;
	friend ostream& operator<<(ostream& out, const StateAi& obj);
};
