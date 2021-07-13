#include "State.h"
#include <nmmintrin.h>
#include "w16v.h"
#include <iomanip>

State::State(int sn) :_u8_num(sn), _u128_vec(((_u8_num - 1) >> 4) + 1,_mm_setzero_si128()) {}
State::State(const vector<WordType>& s) :_u8_num(s.size()), _u128_vec(((_u8_num - 1) >> 4) + 1, _mm_setzero_si128()) {
	for (int i = 0; i < _u8_num; ++i) set(i, s[i]);
	/*int i = 0;
	int r0 = (_u128_vec.size() << 4) - _u8_num;
	int r = 16 - r0;
	for (; i < _u128_vec.size() - 1; i++)
		memcpy(_u128_vec[i].m128i_u8, &s[i << 4], 16);
	memcpy(_u128_vec[i].m128i_u8 + r0, &s[i << 4], r);*/
}

State& State::operator=(const State& rhs) {
	for (int i = 0; i < _u128_vec.size(); i++)
		_u128_vec[i] = rhs._u128_vec[i];
	return *this;
}

State& State::operator^=(const State& rhs) {
	for (int i = 0; i < _u128_vec.size(); i++)
		_u128_vec[i] = _mm_xor_si128(_u128_vec[i], rhs._u128_vec[i]);
	return *this;
}

State& State::operator&=(const State& rhs) {
	for (int i = 0; i < _u128_vec.size(); i++)
		_u128_vec[i] = _mm_and_si128(_u128_vec[i], rhs._u128_vec[i]);
	return *this;
}

const State State::operator^(const State& rhs)const {
	return State(*this) ^= rhs;
}

const State State::operator&(const State& rhs)const {
	return State(*this) &= rhs;
}

__m128i rotl128(__m128i x, int n) {
#define ROTL128(m) case m:return _mm_slli_si128(x, m);
	switch (n) {
	case 0:break;
		ROTL128(1)	ROTL128(2) ROTL128(3)	ROTL128(4) ROTL128(5)	ROTL128(6) ROTL128(7)	ROTL128(8)
			ROTL128(9)	ROTL128(10) ROTL128(11)	ROTL128(12) ROTL128(13)	ROTL128(14) ROTL128(15)
	case 16:_mm_setzero_si128();
	default:_mm_setzero_si128();
	}
	return x;
}
__m128i rotr128(__m128i x, int n) {
#define ROTR128(m) case m:return _mm_srli_si128(x, m);
	switch (n) {
	case 0:break;
		ROTR128(1)	ROTR128(2) ROTR128(3)	ROTR128(4) ROTR128(5)	ROTR128(6) ROTR128(7)	ROTR128(8)
			ROTR128(9)	ROTR128(10) ROTR128(11)	ROTR128(12) ROTR128(13)	ROTR128(14) ROTR128(15)
	case 16:_mm_setzero_si128();
	default:_mm_setzero_si128();
	}
	return x;
}

//TODO: consider the situation that _u8_num is not a multiple of 16. 
State State::operator<<(int n)const {
	if (n < 0) return this->operator>>(-n);
	n %= _u8_num;
	if (n == 0) return *this;
	State out(_u8_num);
	int q = n >> 4;
	int r = n - (q << 4);
	if (r != 0) {
		for (int i = 0; i < _u128_vec.size(); i++) {
			int k = i - q;
			if (k < 0) k += _u128_vec.size();
			int l = k - 1;
			if (l < 0) l += _u128_vec.size();
			out._u128_vec[i] = _mm_xor_si128(rotl128(_u128_vec[k], r), rotr128(_u128_vec[l], 16 - r));
		}
	}
	else {
		for (int i = 0; i < _u128_vec.size(); i++) {
			int k = i - q;
			if (k < 0) k += _u128_vec.size();
			out._u128_vec[i] = _u128_vec[k];
		}
	}
	return out;
}

State State::operator>>(int n)const {
	n %= _u8_num;
	if (n <= 0) return this->operator<<(-n);
	return this->operator<<(_u8_num - n);
}

bool State::operator==(const State& rhs)const {
	for (int i = 0; i < _u128_vec.size(); i++) {
		__m128i tmp = _mm_cmpeq_epi8(_u128_vec[i], rhs._u128_vec[i]);
		if (_mm_movemask_epi8(tmp) != (int)0xffff) return false;
	}
	return true;
}

__m128i _mm_cmpge_epu8(__m128i A, __m128i B) {
	return _mm_cmpeq_epi8(_mm_max_epu8(A, B), A);
}
__m128i _mm_cmple_epu8(__m128i A, __m128i B) {
	return _mm_cmpge_epu8(B, A);
}
__m128i _mm_cmpgt_epu8(__m128i A, __m128i B) {
	return _mm_xor_si128(_mm_cmple_epu8(A, B), _mm_set1_epi8(-1));
}
__m128i _mm_cmplt_epu8(__m128i A, __m128i B) {
	return  _mm_cmpgt_epu8(B, A);
}

bool State::operator>(const State& rhs)const {
	for (int i = _u128_vec.size() - 1; i >= 0; i--) {
		__m128i res_gt_128 = _mm_cmpgt_epu8(_u128_vec[i], rhs._u128_vec[i]);
		__m128i res_lt_128 = _mm_cmplt_epu8(_u128_vec[i], rhs._u128_vec[i]);
		int res_gt = _mm_movemask_epi8(res_gt_128);
		int res_lt = _mm_movemask_epi8(res_lt_128);
		if (res_lt > res_gt) return true;
		else if (res_lt < res_gt) return false;
	}
	return false;
}

bool State::operator>=(const State& rhs)const {
	return (this->operator>(rhs) || this->operator==(rhs));
}

bool State::operator<(const State& rhs)const {
	return !(this->operator>(rhs)) && !(this->operator==(rhs));
}

bool State::operator<=(const State& rhs)const {
	return !(this->operator>(rhs));
}

namespace std {
	template<>
	struct hash<State> {
		size_t operator()(const State& s)const {
			size_t hash_value = 0;
			for (int i = 0; i < s._u128_vec.size(); i++) {
				hash_value = hash<uint64_t>()(hash_value ^ s._u128_vec[i].m128i_u64[0]);
				hash_value = hash<uint64_t>()(hash_value ^ s._u128_vec[i].m128i_u64[1]);
			}
			return hash_value;
		}
	};
}

int popcount64d(uint64_t x){
	int count;
	for (count = 0; x; count++)
		x &= x - 1;
	return count;
}

int State::hw()const {
	int count = 0;
	for (int i = 0; i < _u128_vec.size(); i++) {
		count += popcount64d(_u128_vec[i].m128i_u64[0]);
		count += popcount64d(_u128_vec[i].m128i_u64[1]);
	}
	return count;
}

void State::set(size_t pos, WordType val) {
	int q = pos >> 4;
	int r = pos - (q << 4);
	_u128_vec[_u128_vec.size() - 1 - q].m128i_u8[15 - r] = val;
}

void State::set0() {
	for (int i = 0; i < _u128_vec.size(); i++)
		_u128_vec[i] = _mm_setzero_si128();
}

WordType State::get_word(size_t pos)const {
	int q = pos >> 4;
	int r = pos - (q << 4);
	return _u128_vec[_u128_vec.size() - 1 - q].m128i_u8[15 - r];
}

int State::get_sn()const {
	return _u8_num;
}

int State::get_an()const {
	int sum = 0;
	for (int i = 0; i < _u128_vec.size(); i++) {
		__m128i tmp = _mm_cmpeq_epi8(_u128_vec[i], _mm_setzero_si128());
		unsigned int pattern = _mm_movemask_epi8(tmp);
		pattern = (~pattern) & 0xffff;
		sum += (_mm_popcnt_u32(pattern));
	}
	return sum;
}

vector<int> State::get_ai()const {
	vector<int> indice;
	for (int i = 0; i < _u128_vec.size(); i++) {
		__m128i tmp = _mm_cmpeq_epi8(_u128_vec[_u128_vec.size() - 1 - i], _mm_setzero_si128());
		unsigned int pattern = _mm_movemask_epi8(tmp);
		pattern = (~pattern) & 0xffff;
		int hw = _mm_popcnt_u32(pattern);
		tmp = _mm_add_epi8(*(__m128i*)w16v_r[pattern], _mm_set1_epi8(i << 4));//一共256个S盒以内
		uint8_t* u8_p = (uint8_t*)&tmp;
		indice.insert(indice.end(), u8_p, u8_p + hw);
	}
	return indice;
}

ostream& operator<<(ostream& out, const State& obj) {
	out << std::hex;
	for (int i = 0; i < obj._u8_num; i++)
		out << std::setfill('0') << std::setw(1) << (int)obj.get_word(i);// << " ";
	return out;
}

WordType StateAi::get_word_ai(size_t pos)const {
	return get_word(indice[pos]);
}
int StateAi::get_ai(size_t pos)const {
	return indice[pos];
}
size_t StateAi::get_ai_size()const {
	return indice.size();
}
ostream& operator<<(ostream& out, const StateAi& obj) {
	if (obj.indice.empty()) {
		out << "all zero";
		return out;
	}
	for (int i = 0; i < obj.indice.size(); i++) {
		out << "X[" << std::dec << obj.indice[i] << "]=0x" << std::hex << (int)obj.get_word_ai(i);
		if (i != (obj.indice.size() - 1)) out << ",";
	}
	return out;
}