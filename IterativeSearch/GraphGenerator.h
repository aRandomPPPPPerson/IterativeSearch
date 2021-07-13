#pragma once
#include "Graph.h"
#include "Cipher.h"
class GraphGenerator {
	const Cipher& cipher;
	Graph& graph;
public:
	GraphGenerator(const Cipher& c, Graph& g) :cipher(c), graph(g) {}
	void search(int max_an);
	void search_t(int max_an);
	void search(int max_an, function<State(const State&)> comp_func);
};

State compress(const State& s);
void append(Graph& graph, const State& si, const State& so, const CostType& cost, function<int(const State&)> to_repre);

class MultiStageGraphGenerator {
	const Cipher& cipher;
	MultiStageGraph& multistage_graph;
	Graph graph;
	GraphGenerator graph_generator;
	vector<State> _state;
	vector<CostType> _cost;
	function<bool(const State&)> input_checker;
	function<bool(const State&)> output_checker;
	void expand(int j, WeightType ws, const vector<WeightType>& wb, bool flag_fw);
	void restore_fw(int j);
	void restore_bw(int j);
	void generate(int r, int max_an, const vector<WeightType>& wb_fw, const vector<WeightType>& wb_bw);
public:
	MultiStageGraphGenerator(const Cipher& __cipher, MultiStageGraph& __msg) :cipher(__cipher), multistage_graph(__msg), graph_generator(cipher, graph) {}
	void generate(int r, int max_an, int rd, WeightType ws);
};