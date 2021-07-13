#include <vector>
using std::vector;
#include "Types.h"

//SPONGENT
template<int b>
constexpr int perm_b_spongent(int j) {
	if (j == b - 1) return b - 1;
	return (j * (b >> 2)) % (b - 1);
}
template<int b>
vector<WordType> linearTransform_bitpermutation_spongent(const vector<WordType>& in, int sbox_size) {
	int sbox_num = b / sbox_size;
	WordType mask = 1 << (sbox_size - 1);
	vector<WordType> out(sbox_num);
	for (int i = 0; i < sbox_num; i++)
		for (int j = 0; j < sbox_size; j++) {
			int out_index = perm_b_spongent<b>(sbox_size - 1 - j + sbox_size * i);
			out[out_index / sbox_size] ^= ((in[i] << j) & mask) >> (sbox_size - 1 - out_index % sbox_size);
		}
	return out;
}
template<int b>
vector<WordType> linearTransformInv_bitpermutation_spongent(const vector<WordType>& in, int sbox_size) {
	int sbox_num = b / sbox_size;
	WordType mask = 1 << (sbox_size - 1);
	vector<WordType> out(sbox_num);
	for (int i = 0; i < sbox_num; i++)
		for (int j = 0; j < sbox_size; j++) {
			int out_index = perm_b_spongent<b>(perm_b_spongent<b>(sbox_size - 1 - j + sbox_size * i));
			out[out_index / sbox_size] ^= ((in[i] << j) & mask) >> (sbox_size - 1 - out_index % sbox_size);
		}
	return out;
}
template<int b>
vector<WordType> linearTransform_spongent(const vector<WordType>& in) {
	return linearTransform_bitpermutation_spongent<b>(in, 4);
}
template<int b>
vector<WordType> linearTransformInv_spongent(const vector<WordType>& in) {
	return linearTransformInv_bitpermutation_spongent<b>(in, 4);
}