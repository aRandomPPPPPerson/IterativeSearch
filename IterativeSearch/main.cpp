#include <iostream>
using std::cout;
using std::endl;

#include "Cipher.h"
#include "Graph.h"
#include "IterativeSearch.h"

int main() {
	unique_ptr<Cipher> cipher_ptr = make_cipher("iceberg", AttackType::LAT2);
	cipher_ptr->print();
	IterativeSearcher iterative_searcher(*cipher_ptr);
	iterative_searcher.search_iterative(1);
	//iterative_searcher.search_distinguisher(13, 2, 3, 12);

	return 0;
}