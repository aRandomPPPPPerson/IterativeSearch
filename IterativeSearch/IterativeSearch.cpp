#include "IterativeSearch.h"
#include <fstream>
using std::ofstream;
#include <iostream>
#include "Timer.h"

void IterativeSearcher::gen_refined() {
	set<State> vertices;
	for (auto it_si = graph.head_tail_distance_trails_table.begin(); it_si != graph.head_tail_distance_trails_table.end(); it_si++) {
		vertices.insert(it_si->first);
		for (auto it_so = it_si->second.tail_distance_trails_table.begin(); it_so != it_si->second.tail_distance_trails_table.end(); it_so++) {
			vertices.insert(it_so->first);
		}
	}
	int i = 0;
	for (auto it = vertices.begin(); it != vertices.end(); it++, i++) {
		state_mark_map.insert(make_pair(*it, i));
		mark_state_map.insert(make_pair(i, *it));
	}
	for (auto it_si = graph.head_tail_distance_trails_table.begin(); it_si != graph.head_tail_distance_trails_table.end(); it_si++) {
		for (auto it_so = it_si->second.tail_distance_trails_table.begin(); it_so != it_si->second.tail_distance_trails_table.end(); it_so++) {
			refined_graph.insert(state_mark_map.find(it_si->first)->second, state_mark_map.find(it_so->first)->second, it_so->second.get_min_weight());
		}
	}
}

void IterativeSearcher::search_iterative(int max_an, const string& method) {
	ofstream outfile;
	string name_iterative = cipher.name + "_" + std::to_string(max_an) + "_iterative.txt";
	outfile.open(name_iterative);
	Timer timer;

	std::cout << "Generating ..." << std::endl;
	timer.tik();
	if (method == "normal")
		graph_generator.search(max_an);
	else if (method == "total")
		graph_generator.search_t(max_an);
	timer.tok();
	outfile << "Generate graph in " << timer.tiktok() << "\n";


	std::cout << "Reducing ..." << std::endl;
	outfile << "Before reducing" << std::endl;
	outfile << "|V|=" << graph.get_vertex_num() << "\t|E|=" << graph.get_edge_num() << std::endl;
	graph.reduce();
	outfile << "After reducing" << std::endl;
	outfile << "|V|=" << graph.get_vertex_num() << "\t|E|=" << graph.get_edge_num() << std::endl;

	gen_refined();
	std::cout << "Circuiting ..." << std::endl;
	bool flag_large = graph.get_vertex_num() > 2500;
	if (!flag_large) {
		circuits_searcher.compute();
		print_advanced_graphviz(cipher.name + "_" + std::to_string(max_an));
	}
	else
		std::cout << "The graph is too large. We don't search circuits. " << std::endl;

	if (!flag_large && circuits_searcher.top_circuits.empty())
		outfile << "No circuit found. " << std::endl;
	else {
		print_circuits(outfile);
		std::cout << "Clustering ..." << std::endl;
		vector<WeightType> bw = graph.find_best_iterative(10);
		/*outfile << bw[4] << "\t" << bw[9] << std::endl;
		for (int i = 0; i < bw.size(); i++) {
			outfile << i + 1 << ":" << bw[i] << std::endl;
		}*/
		WeightType bbw = bw[0];
		for (int i = 0; i < bw.size(); i++) {
			if (i == 4 || i == 9) outfile << "*";
			bbw = std::min(bbw, bw[i]);
			outfile << i + 1 << ":" << bbw << std::endl;
		}
	}
	outfile.close();
}

void IterativeSearcher::print_circuits(ostream& out) {
	int i = 0;
	for (auto it1 = circuits_searcher.top_circuits.begin(); it1 !=circuits_searcher.top_circuits.end(); it1++, i++) {
		out << "No." << std::dec << i << "\tweight=" << it1->first << "\n";
		for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
			out << (StateAi)mark_state_map.find(*it2)->second << "\n";
		}
		out << "\n";
	}
}

void IterativeSearcher::print_graphviz(string filename) {
	ofstream outfile;
	outfile.open(filename + ".gv");
	outfile << "digraph G{\n";
	for (auto it1 = refined_graph.adj_list_map.begin(); it1 != refined_graph.adj_list_map.end(); it1++) {
		for (auto it2 = it1->second.adj_list.begin(); it2 != it1->second.adj_list.end(); it2++) {
			outfile << it1->first << " -> " << it2->first << "\n";
		}
	}
	outfile << "}";
	outfile.close();
}

void IterativeSearcher::print_advanced_graphviz(string filename) {
	ofstream outfile;
	outfile.open(filename + ".gv");
	StrongComponentsComputer scc(refined_graph);
	scc.compute_ge(0);
	outfile << "digraph G{\n";
	outfile << std::dec;
	for (auto it = refined_graph.adj_list_map.begin(); it != refined_graph.adj_list_map.end(); it++) {
		outfile << std::dec <<it->first;
		outfile <<" [label=\""<< (StateAi)mark_state_map.find(it->first)->second;
		outfile << "\"]";
		if (scc.in_components(it->first)) {
			outfile << " [color=red];\n";
		}
		else {
			outfile << " [color=blue];\n";
		}
	}
	for (auto it1 = refined_graph.adj_list_map.begin(); it1 != refined_graph.adj_list_map.end(); it1++) {
		for (auto it2 = it1->second.adj_list.begin(); it2 != it1->second.adj_list.end(); it2++) {
			outfile << std::dec << it1->first << " -> " << it2->first;
			outfile << " [label=\"" << std::dec;
			auto it3 = graph.head_tail_distance_trails_table.find(mark_state_map.find(it1->first)->second)->second.tail_distance_trails_table.find(mark_state_map.find(it2->first)->second);
			for (auto it4 = it3->second.distance_trails_table.begin(); it4 != it3->second.distance_trails_table.end(); ) {
				if (cipher.irs == true) outfile << "d=" << it4->first << ":";
				outfile << "w=" << it4->second.total_cost.weight;
				if (++it4 != it3->second.distance_trails_table.end()) outfile << "\\n";
			}
			outfile << "\"]";
			if (scc.in_same_component(it1->first, it2->first)) {
				outfile << " [color=red];";
			}
			else {
				outfile << " [color=blue];";
			}
			outfile << "\n";
		}
	}
	outfile << "}";
	outfile.close();
}

void IterativeSearcher::search_distinguisher(int r, int max_an, int rd, WeightType ws) {
	ofstream outfile;
	string name_distinguisher = cipher.name + "_" + std::to_string(r) + "_" + std::to_string(max_an) + "_" + std::to_string(rd) + "_" + std::to_string(ws) + "_distinguisher.txt";
	outfile.open(name_distinguisher);

	Timer timer;
	timer.tik();
	std::cout << "Generating MSG" << std::endl;
	msg_generator.generate(r, max_an, rd, ws);
	timer.tok();
	outfile << "msg generation time = " << timer.tiktok() << std::endl;

	timer.tik();
	std::cout << "Finding the best single charatieristic" << std::endl;
	BestAnswer ba_single = multistage_graph.get_best_single();
	timer.tok();
	outfile << "find the best single characteristic in " << timer.tiktok() << std::endl;
	outfile << ba_single << std::endl;
	
	timer.tik();
	std::cout << "Finding the best cluster" << std::endl;
	BestAnswer ba_cluster = multistage_graph.get_best_cluster();
	timer.tok();
	outfile << "find the best cluster in " << timer.tiktok() << std::endl;
	outfile << ba_cluster << std::endl;

	outfile.close();
}