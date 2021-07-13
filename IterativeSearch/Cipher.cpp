#include "Cipher.h"
#include <fstream>
using std::ofstream;

int to_repre_rotl_num(const State& s) {
	vector<int> indice(s.get_ai());
	int an = indice.size();
	if (an == 0) return 0;
	if (an == 1) return indice[0];
	State best_s(s << indice[0]);
	int best_rotl(indice[0]);
	for (int i = 1; i < an; i++) {
		State cur_s(s << indice[i]);
		if (cur_s > best_s) {
			best_s = cur_s;
			best_rotl = indice[i];
		}
	}
	return best_rotl;
}

int to_repre_always0(const State& s) {
	return 0;
}

unique_ptr<Cipher> make_cipher(const string& cipher_name, AttackType at) {
	string attack_name;
	if (at == AttackType::DDT) attack_name = "_DDT";
	else if (at == AttackType::LAT2) attack_name = "_LAT2";
	else if (at == AttackType::LAT) attack_name = "_LAT";
	string name = cipher_name + attack_name;

	if (cipher_name == "knot256")
		return std::make_unique<Cipher>(s_knot, 4, 4, at, 64, linearTransform_knot256, linearTransformInv_knot256, true, name);
	if (cipher_name == "knot384")
		return std::make_unique<Cipher>(s_knot, 4, 4, at, 96, linearTransform_knot384, linearTransformInv_knot384, true, name);
	if (cipher_name == "knot512")
		return std::make_unique<Cipher>(s_knot, 4, 4, at, 128, linearTransform_knot512, linearTransformInv_knot512, true, name);
	if (cipher_name == "rectangle")
		return std::make_unique<Cipher>(s_rectangle, 4, 4, at, 16, linearTransform_rectangle, linearTransformInv_rectangle, true, name);
	if (cipher_name == "present")
		return std::make_unique<Cipher>(s_present, 4, 4, at, 16, linearTransform_present, linearTransformInv_present, false, name);
	if (cipher_name == "gift64")
		return std::make_unique<Cipher>(s_gift, 4, 4, at, 16, linearTransform_gift64, linearTransformInv_gift64, false, name);
	if (cipher_name == "gift128")
		return std::make_unique<Cipher>(s_gift, 4, 4, at, 32, linearTransform_gift128, linearTransformInv_gift128, false, name);
	if (cipher_name == "puffin")
		return std::make_unique<Cipher>(s_puffin, 4, 4, at, 16, linearTransform_puffin, linearTransformInv_puffin, false, name);
	if (cipher_name == "trifle")
		return std::make_unique<Cipher>(s_trifle, 4, 4, at, 32, linearTransform_trifle, linearTransformInv_trifle, false, name);
	if (cipher_name == "ascon")
		return std::make_unique<Cipher>(s_ascon, 5, 5, at, 64, linearTransform_ascon, linearTransformInv_ascon, true, name);
	if (cipher_name == "spongent128")
		return std::make_unique<Cipher>(s_spongent, 4, 4, at, 32, linearTransform_spongent128, linearTransformInv_spongent128, false, name);
	if (cipher_name == "spongent176")
		return std::make_unique<Cipher>(s_spongent, 4, 4, at, 44, linearTransform_spongent176, linearTransformInv_spongent176, false, name);
	if (cipher_name == "epcbc48")
		return std::make_unique<Cipher>(s_epcbc, 4, 4, at, 12, linearTransform_epcbc48, linearTransformInv_epcbc48, false, name);
	if (cipher_name == "epcbc96")
		return std::make_unique<Cipher>(s_epcbc, 4, 4, at, 24, linearTransform_epcbc96, linearTransformInv_epcbc96, false, name);
	if (cipher_name == "pride")
		return std::make_unique<Cipher>(s_pride, 4, 4, at, 16, linearTransform_pride, linearTransformInv_pride, false, name);
	if (cipher_name == "iceberg")
		return std::make_unique<Cipher>(s_iceberg, 8, 8, at, 8, linearTransform_iceberg, linearTransformInv_iceberg, false, name);
	return nullptr;
}

void Cipher::print()const {
	string filename_fw = name + "_fw.txt";
	string filename_bw = name + "_bw.txt";
	ofstream outfile;
	outfile.open(filename_fw);
	outfile << spl_fw;
	outfile.close();
	outfile.open(filename_bw);
	outfile << spl_bw;
	outfile.close();
}