#pragma once
#include "SPLayer.h"
#include <string>
using std::string;
using std::unique_ptr;

int to_repre_rotl_num(const State& s);
int to_repre_always0(const State& s);

struct Cipher {
	string name;
	SPLayer spl_fw;
	SPLayer spl_bw;
	bool irs;
	function<int(const State&)> to_repre;

	template <class RandomAccessIterator>
	Cipher(RandomAccessIterator first, int ibn, int obn, AttackType at, int sn, function<vector<WordType>(const vector<WordType>&)> fw, function<vector<WordType>(const vector<WordType>&)> bw, bool is_irs, const string& __name) :
		spl_fw(first, ibn, obn, at, sn, fw), spl_bw(spl_fw.inverse_sbox(), at, sn, bw), irs(is_irs),name(__name) {
		to_repre = irs == true ? to_repre_rotl_num : to_repre_always0;
	}
	void print()const;
};

unique_ptr<Cipher> make_cipher(const string& cipher_name, AttackType at);