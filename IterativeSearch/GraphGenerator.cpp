#include "GraphGenerator.h"
#include "Traverser.h"
void GraphGenerator::search(int max_an) {
	int sn = cipher.spl_fw.get_sn();
	for (int an = 1; an <= max_an; an++) {
		State si(sn);
		for (InputTraverser it(si, an, cipher.spl_fw.get_ibn(), cipher.irs); !it.end(); it.next()) {
			State s_bp(cipher.spl_bw.p_lookup(si));
			int an_bp = s_bp.get_an();
			if (an_bp > max_an || an_bp < an) continue;
			int si_shift = cipher.to_repre(si);
			State so(sn);
			for (OutputTraverser ot(so, 0, INF, si, cipher.spl_fw); !ot.end(); ot.next()) {
				if (ot.output_state.get_an() <= max_an) {
					int so_shift = cipher.to_repre(so);
					int d = (so_shift - si_shift);
					if (d < 0) d += sn;
					graph.insert(si << si_shift, so << so_shift, d, ot.get_cost());
				}
			}
		}
	}
}

void GraphGenerator::search_t(int max_an) {
	int sn = cipher.spl_fw.get_sn();
	for (int an = 1; an <= max_an/2; an++) {
		State si(sn);
		for (InputTraverser it(si, an, cipher.spl_fw.get_ibn(), cipher.irs); !it.end(); it.next()) {
			State s_bp(cipher.spl_bw.p_lookup(si));
			if (s_bp.get_an() > max_an) continue;
			int si_shift = cipher.to_repre(si);
			State so(sn);
			for (OutputTraverser ot(so, 0, INF, si, cipher.spl_fw); !ot.end(); ot.next()) {
				if (ot.output_state.get_an()+an <= max_an) {
					int so_shift = cipher.to_repre(so);
					int d = (so_shift - si_shift);
					if (d < 0) d += sn;
					graph.insert(si << si_shift, so << so_shift, d, ot.get_cost());
				}
			}


			//TODO: 2-4,4-2
		}
	}
}

/*State compress(const State& s) {
	State t(s.get_sn());
	__m128i m = _mm_set1_epi8(0xff);
	for (int i = 0; i < s._u128_vec.size(); i++) {
		t._u128_vec[i].word128 = _mm_xor_si128(_mm_cmpeq_epi8(s._u128_vec[i].word128, _mm_setzero_si128()), m);
	}
	return t;
}*/

void GraphGenerator::search(int max_an, function<State(const State&)> comp_func) {
	int sn = cipher.spl_fw.get_sn();
	for (int an = 1; an <= max_an; an++) {
		State si(sn);
		for (InputTraverser it(si, an, cipher.spl_fw.get_ibn(), cipher.irs); !it.end(); it.next()) {
			State s_bp(cipher.spl_bw.p_lookup(si));
			if (s_bp.get_an() > max_an) continue;
			int si_shift = cipher.to_repre(si);
			State ssi(si << si_shift);
			State so(sn);
			for (OutputTraverser ot(so, 0, INF, si, cipher.spl_fw); !ot.end(); ot.next()) {
				if (ot.output_state.get_an() <= max_an) {
					int so_shift = cipher.to_repre(so);
					State sso(so << so_shift);
					int d = (so_shift - si_shift);
					if (d < 0) d += sn;
					graph.insert(comp_func(ssi), comp_func(sso), d, CostType());
				}
			}
		}
	}
}

void append(Graph& graph, const State& si, const State& so, const CostType& cost, function<int(const State&)> to_repre) {
	int __di = to_repre(si);
	int __do = to_repre(so);
	int d = __do - __di;
	if (d < 0) d += si.get_sn();
	graph.insert(si << __di, so << __do, d, cost);
}

vector<WeightType> gen_wb(int rd, WeightType ws, WeightType wrmin) {
	vector<WeightType> wb(rd);
	wb[rd - 1] = ws;
	for (int i = rd - 2; i >= 0; i--) {
		wb[i] = wb[i + 1] - wrmin;
	}
	return wb;
}

void MultiStageGraphGenerator::generate(int r, int max_an, int rd, WeightType ws) {
	generate(r, max_an, gen_wb(rd, ws, cipher.spl_fw.get_min_weight()), gen_wb(rd, ws, cipher.spl_fw.get_min_weight()));
}

void MultiStageGraphGenerator::generate(int r, int max_an, const vector<WeightType>& wb_fw, const vector<WeightType>& wb_bw) {
	graph_generator.search(max_an);
	graph.reduce();
	multistage_graph.graphs = vector<Graph>(r, graph);

	for (auto it = graph.head_tail_distance_trails_table.begin(); it != graph.head_tail_distance_trails_table.end(); it++) {
		_state = vector<State>(wb_fw.size() + 1, cipher.spl_fw.get_sn());
		_cost = vector<CostType>(wb_fw.size());
		_state[0] = it->first;
		expand(1, 0, wb_fw, true);

		_state = vector<State>(wb_bw.size() + 1, cipher.spl_fw.get_sn());
		_cost = vector<CostType>(wb_bw.size());
		_state[0] = cipher.spl_bw.p_lookup(it->first);
		expand(1, 0, wb_bw, false);
	}
}

void MultiStageGraphGenerator::expand(int j, WeightType ws, const vector<WeightType>& wb, bool fw_flag) {

	int rd_r = wb.size() - j;
	WeightType w_r = wb[j - 1] - ws;
	for (OutputTraverser otc(_state[j], 0, w_r + 0.01, _state[j - 1], fw_flag ? cipher.spl_fw : cipher.spl_bw); !otc.end(); otc.next()) {
		_cost[j - 1] = otc.get_cost();
		if (fw_flag) {
			restore_fw(j);
		}
		else
			restore_bw(j);
		if (rd_r > 0) expand(j + 1, ws + _cost[j - 1].weight, wb, fw_flag);
	}
}

//TODO reduce << operations
void MultiStageGraphGenerator::restore_fw(int j) {
	for (int i = j - 1; i >= 0; i--) {
		if (i == j - 1) if (output_checker != nullptr && output_checker(_state[j])) return;
		append(multistage_graph.graphs[multistage_graph.graphs.size() - j + i], _state[i], _state[i + 1], _cost[i], cipher.to_repre);
	}
}

void MultiStageGraphGenerator::restore_bw(int j) {
	for (int i = j - 1; i >= 0; i--) {
		if (i == j - 1) if (input_checker != nullptr && input_checker(cipher.spl_fw.p_lookup(_state[j]))) return;
		append(multistage_graph.graphs[j - 1 - i], cipher.spl_fw.p_lookup(_state[i + 1]), cipher.spl_fw.p_lookup(_state[i]), _cost[i], cipher.to_repre);
	}
}