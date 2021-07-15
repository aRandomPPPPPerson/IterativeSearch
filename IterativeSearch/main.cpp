#include <iostream>
using std::cout;
using std::endl;

#include "IterativeSearch.h"

int main() {
	//gift64, present, rectangle
	string cipher_name = "gift64";
	//DDT, LAT2
	AttackType att = AttackType::DDT;
	//parameters
	int max_an = 2;
	int rd = 13;
	int rd_extension = 3;
	int wt_extension = 12;

	unique_ptr<Cipher> cipher_ptr = make_cipher(cipher_name, att);
	cipher_ptr->print();
	IterativeSearcher iterative_searcher(*cipher_ptr);
	iterative_searcher.search_iterative(max_an);
	iterative_searcher.search_distinguisher(rd, max_an, rd_extension, wt_extension);
	return 0;
}