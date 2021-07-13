#pragma once
#include "GraphGenerator.h"
#include "Circuits.h"



class IterativeSearcher {
	const Cipher& cipher;
	Graph graph;
	GraphGenerator graph_generator;

	AdjacencyStructure refined_graph;
	CircuitsComputer circuits_searcher;
	map<int, State> mark_state_map;
	map<State, int> state_mark_map;

	MultiStageGraph multistage_graph;
	MultiStageGraphGenerator msg_generator;
	void gen_refined();
public:
	IterativeSearcher(const Cipher& c):cipher(c),graph_generator(cipher,graph), circuits_searcher(refined_graph), msg_generator(c, multistage_graph){}
	void search_iterative(int max_an, const string& method = "normal");
	void search_distinguisher(int r, int max_an, int rd, WeightType ws);
	
	void print_circuits(ostream& out);
	void print_graphviz(string filename);
	void print_advanced_graphviz(string filename);
};