#include <iostream>
using std::cout;
using std::endl;

#include "Cipher.h"
#include "Graph.h"
#include "IterativeSearch.h"

int main() {
	unique_ptr<Cipher> cipher_ptr = make_cipher("present", AttackType::DDT);
	cipher_ptr->print();
	IterativeSearcher iterative_searcher(*cipher_ptr);
	iterative_searcher.search_iterative(2);
	iterative_searcher.search_distinguisher(16, 2, 3, 10);

	return 0;
}