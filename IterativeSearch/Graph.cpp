#include "Graph.h"
#include <algorithm>
#include <set>
using std::set;
#include <iostream>
using std::cout;
using std::endl;

TrailCount TrailCount::operator+(const TrailCount& rhs)const {
	TrailCount tc = rhs;
	for (auto it = trail_count.begin(); it != trail_count.end(); it++) {
		auto ret = tc.trail_count.insert(*it);
		if (ret.second == false)
			ret.first->second += it->second;
	}
	return tc;
}
TrailCount TrailCount::operator*(const TrailCount& rhs)const {
	TrailCount tc;
	for (auto it1 = trail_count.begin(); it1 != trail_count.end(); it1++) {
		for (auto it2 = rhs.trail_count.begin(); it2 != rhs.trail_count.end(); it2++) {
			CostType cost = it1->first * it2->first;
			int count = it1->second * it2->second;
			auto ret = tc.trail_count.insert(make_pair(cost, count));
			if (ret.second == false)
				ret.first->second += count;
		}
	}
	return tc;
}
ostream& operator<<(ostream& out, const TrailCount& obj) {
	if (obj.trail_count.empty()) return out;
	out << "num of trails=\n";
	for (auto it = obj.trail_count.begin(); it != obj.trail_count.end(); it++) {
		out << it->first << "\t" << it->second << "\n";
	}
	return out;
}


Trails Trails::operator+(const Trails& rhs)const {
	Trails ti;
	ti.total_cost = total_cost + rhs.total_cost;
	ti.trails = trails + rhs.trails;
	return ti;
}
Trails& Trails::operator+=(const Trails& rhs) {
	*this = *this + rhs;
	return *this;
}
Trails Trails::operator*(const Trails& rhs)const {
	Trails ti;
	ti.total_cost = total_cost * rhs.total_cost;
	ti.trails = trails * rhs.trails;
	return ti;
}
Trails& Trails::operator*=(const Trails& rhs) {
	*this = *this * rhs;
	return *this;
}
bool Trails::operator<(const Trails& rhs)const {
	return total_cost < rhs.total_cost;
}
ostream& operator<<(ostream& out, const Trails& obj) {
	out << "cost=" << obj.total_cost << "\n";
	out << obj.trails;
	return out;
}

void DistanceTrailsTable::add(int d, Trails ts) {
	auto ret = distance_trails_table.insert(make_pair(d, ts));
	if (ret.second == false)
		ret.first->second += ts;
}
WeightType DistanceTrailsTable::get_min_weight()const {
	return std::min_element(distance_trails_table.begin(), distance_trails_table.end(),
		[](const std::pair<int, Trails>& a, const std::pair<int, Trails>& b)
		{return a.second.total_cost.weight < b.second.total_cost.weight; }
	)->second.total_cost.weight;
}
ostream& operator<<(ostream& out, const DistanceTrailsTable& obj) {
	for (auto it = obj.distance_trails_table.begin(); it != obj.distance_trails_table.end(); it++)
		out << "d=" << std::dec << it->first << "\n" << it->second << "\n";
	return out;
}



void TailDistanceTrailsTable::add(const State& tail, int d, Trails ts) {
	auto ret = tail_distance_trails_table.insert(make_pair(tail, DistanceTrailsTable(d, ts)));
	if (ret.second == false)
		ret.first->second.add(d, ts);
}
ostream& operator<<(ostream& out, const TailDistanceTrailsTable& obj) {
	for (auto it = obj.tail_distance_trails_table.begin(); it != obj.tail_distance_trails_table.end(); it++)
		out << "so=" << std::dec << (StateAi)it->first << "\n" << it->second << "\n";
	return out;
}

void BestAnswer::insert(const State& __si, const State& __so, int __d, const Trails& __ts) {
	if (state.empty()) {
		state.emplace_back(__si);
		state.emplace_back(__so);
		distance = __d;
		trails = __ts;
	}
	else {
		if (__ts.total_cost.weight < trails.total_cost.weight) {
			state[0] = __si;
			state[1] = __so;
			distance = __d;
			trails = __ts;
		}
	}
}

void BestAnswer::insert(const BestAnswer& __ba) {
	if (!__ba.state.empty()) {
		insert(__ba.state[0], __ba.state[1], __ba.distance, __ba.trails);
	}
}

ostream& operator<<(ostream& out, const BestAnswer& obj) {
	if (obj.state.empty()) {
		out << "best answer not found.\n";
		return out;
	}
	out << "best answer\ninput:\n" << obj.state[0] << "\n" << (StateAi)obj.state[0] << "\n";
	out << "output:\td=" << std::dec << obj.distance << "\n" << obj.state[1] << "\n" << (StateAi)obj.state[1] << "\n";
	out << "output:\n" << (obj.state[1] >> obj.distance) << "\n" << (StateAi)(obj.state[1] >> obj.distance) << "\ntrail info:\n" << obj.trails;
	return out;
}

void HeadTailDistanceTrailsTable::add(const HeadTailDistanceTrailsTable& rhs) {
	HeadTailDistanceTrailsTable new_cost;
	int sbox_num = head_tail_distance_trails_table.begin()->first.get_sn();
	for (auto it1 = head_tail_distance_trails_table.begin(); it1 != head_tail_distance_trails_table.end(); it1++) {
		for (auto it2 = it1->second.tail_distance_trails_table.begin(); it2 != it1->second.tail_distance_trails_table.end(); it2++) {
			auto it3 = rhs.head_tail_distance_trails_table.find(it2->first);
			if (it3 == rhs.head_tail_distance_trails_table.end()) continue;
			for (auto it4 = it3->second.tail_distance_trails_table.begin(); it4 != it3->second.tail_distance_trails_table.end(); it4++) {
				for (auto it5 = it2->second.distance_trails_table.begin(); it5 != it2->second.distance_trails_table.end(); it5++) {
					for (auto it6 = it4->second.distance_trails_table.begin(); it6 != it4->second.distance_trails_table.end(); it6++) {
						int shift = (it5->first + it6->first) % sbox_num;
						new_cost.add(it1->first, it4->first, shift, it5->second * it6->second);
					}
				}
			}
		}
	}
	this->head_tail_distance_trails_table.swap(new_cost.head_tail_distance_trails_table);
}
void HeadTailDistanceTrailsTable::add(const HeadTailDistanceTrailsTable& rhs, const State& constant) {
	HeadTailDistanceTrailsTable new_cost;
	int sbox_num = head_tail_distance_trails_table.begin()->first.get_sn();
	for (auto it1 = head_tail_distance_trails_table.begin(); it1 != head_tail_distance_trails_table.end(); it1++) {
		for (auto it2 = it1->second.tail_distance_trails_table.begin(); it2 != it1->second.tail_distance_trails_table.end(); it2++) {
			auto it3 = rhs.head_tail_distance_trails_table.find(it2->first);
			if (it3 == rhs.head_tail_distance_trails_table.end()) continue;
			for (auto it4 = it3->second.tail_distance_trails_table.begin(); it4 != it3->second.tail_distance_trails_table.end(); it4++) {
				for (auto it5 = it2->second.distance_trails_table.begin(); it5 != it2->second.distance_trails_table.end(); it5++) {
					for (auto it6 = it4->second.distance_trails_table.begin(); it6 != it4->second.distance_trails_table.end(); it6++) {
						int shift = (it5->first + it6->first) % sbox_num;
						int hw = ((it4->first >> shift) & constant).hw();
						Trails ts = it5->second * it6->second;
						ts.total_cost.sign ^= (hw & 1);
						new_cost.add(it1->first, it4->first, shift, ts);
					}
				}
			}
		}
	}
	this->head_tail_distance_trails_table.swap(new_cost.head_tail_distance_trails_table);
}
void HeadTailDistanceTrailsTable::add(const State& head, const State& tail, int d, Trails ts) {
	auto ret = head_tail_distance_trails_table.insert(make_pair(head, TailDistanceTrailsTable(tail, d, ts)));
	if (ret.second == false)
		ret.first->second.add(tail, d, ts);
}
BestAnswer HeadTailDistanceTrailsTable::get_best()const {
	BestAnswer ba;
	for (auto it1 = head_tail_distance_trails_table.begin(); it1 != head_tail_distance_trails_table.end(); it1++) {
		for (auto it2 = it1->second.tail_distance_trails_table.begin(); it2 != it1->second.tail_distance_trails_table.end(); it2++) {
			for (auto it3 = it2->second.distance_trails_table.begin(); it3 != it2->second.distance_trails_table.end(); it3++) {
				ba.insert(it1->first, it2->first, it3->first, it3->second);
			}
		}
	}
	return ba;
}
BestAnswer HeadTailDistanceTrailsTable::get_best_iterative()const {
	BestAnswer ba;
	for (auto it1 = head_tail_distance_trails_table.begin(); it1 != head_tail_distance_trails_table.end(); it1++) {
		for (auto it2 = it1->second.tail_distance_trails_table.begin(); it2 != it1->second.tail_distance_trails_table.end(); it2++) {
			if (!(it1->first == it2->first)) continue;
			for (auto it3 = it2->second.distance_trails_table.begin(); it3 != it2->second.distance_trails_table.end(); it3++) {
				ba.insert(it1->first, it2->first, it3->first, it3->second);
			}
		}
	}
	return ba;
}
vector<WeightType> HeadTailDistanceTrailsTable::find_best_iterative(int r)const {
	vector<WeightType> bwpr(r, get_best_iterative().trails.total_cost.weight);
	int total_count = head_tail_distance_trails_table.size();
	int percent = 0;
	int percent_former = percent;
	cout<<"The graph has " <<total_count<<" while searching iterative"<< endl;
	int count = 0;
	for (auto it = head_tail_distance_trails_table.begin(); it != head_tail_distance_trails_table.end(); it++) {
		count++;
		percent = count * 100 / total_count;
		if (percent != percent_former) {
			std::cout << "searching " << std::dec << percent << "%\n";
			percent_former = percent;
		}
		HeadTailDistanceTrailsTable total;
		total.head_tail_distance_trails_table.insert(*it);
		for (int i = 1; i < r; i++) {
			total.add(*this);
			WeightType w = total.get_best_iterative().trails.total_cost.weight;
			WeightType wpr = w == INF ? INF : w / (i + 1);
			bwpr[i] = std::min(wpr, bwpr[i]);
		}
	}
	return bwpr;
}
bool HeadTailDistanceTrailsTable::_reduce() {
	set<State> probed;
	bool changed = false;
	for (auto it1 = head_tail_distance_trails_table.begin(); it1 != head_tail_distance_trails_table.end();) {
		for (auto it2 = it1->second.tail_distance_trails_table.begin(); it2 != it1->second.tail_distance_trails_table.end();) {
			auto it_refind = head_tail_distance_trails_table.find(it2->first);
			if (it_refind == head_tail_distance_trails_table.end()) {
				it1->second.tail_distance_trails_table.erase(it2++);
				changed = true;
			}
			else {
				for (auto it3 = it_refind->second.tail_distance_trails_table.begin(); it3 != it_refind->second.tail_distance_trails_table.end(); it3++) {
					probed.insert(it3->first);
				}
				it2++;
			}
		}
		if (it1->second.tail_distance_trails_table.empty()) {
			head_tail_distance_trails_table.erase(it1++);
		}
		else {
			it1++;
		}
	}
	for (auto it1 = head_tail_distance_trails_table.begin(); it1 != head_tail_distance_trails_table.end();) {
		if (probed.find(it1->first) == probed.end()) {
			changed = true;
			head_tail_distance_trails_table.erase(it1++);
		}
		else {
			it1++;
		}
	}
	return changed;
}
void HeadTailDistanceTrailsTable::reduce() { 
	while (_reduce()) {} 
}
ostream& operator<<(ostream& out, const HeadTailDistanceTrailsTable& obj) {
	for (auto it = obj.head_tail_distance_trails_table.begin(); it != obj.head_tail_distance_trails_table.end(); it++)
		out << "si=" << std::dec << (StateAi)it->first << "\n" << it->second << "\n";
	return out;
}

BestAnswer HeadTailDistanceTrailsTable::get_best(function<bool(const State&, const State&)> checker, function<State(const State&)> output_truncator, function<int(const State&)> to_repre)const {
	HeadTailDistanceTrailsTable temp;
	if (head_tail_distance_trails_table.empty()) return BestAnswer();
	int sn = head_tail_distance_trails_table.begin()->first.get_sn();
	State zero(sn);
	for (auto it1 = head_tail_distance_trails_table.begin(); it1 != head_tail_distance_trails_table.end(); it1++) {
		for (auto it2 = it1->second.tail_distance_trails_table.begin(); it2 != it1->second.tail_distance_trails_table.end(); it2++) {
			State so(output_truncator(it2->first));
			bool so_is_zero = so == zero;
			int d = so_is_zero ? 0 : to_repre(so);
			for (auto it3 = it2->second.distance_trails_table.begin(); it3 != it2->second.distance_trails_table.end(); it3++) {
				if (so_is_zero) temp.add(it1->first, so, 0, it3->second);
				else {
					if (checker != nullptr && !checker(it1->first, so >> it3->first)) continue;
					int dd = it3->first + d;
					if (dd >= sn) dd -= sn;
					temp.add(it1->first, so << d, dd, it3->second);
				}
			}
		}
	}

	return temp.get_best();
}

int HeadTailDistanceTrailsTable::get_edge_num()const {
	int count = 0;
	for (auto it1 = head_tail_distance_trails_table.begin(); it1 != head_tail_distance_trails_table.end(); it1++) {
		for (auto it2 = it1->second.tail_distance_trails_table.begin(); it2 != it1->second.tail_distance_trails_table.end(); it2++) {
			count++;
		}
	}
	return count;
}
int HeadTailDistanceTrailsTable::get_vertex_num()const {
	set<State> vertex_set;
	for (auto it1 = head_tail_distance_trails_table.begin(); it1 != head_tail_distance_trails_table.end(); it1++) {
		vertex_set.insert(it1->first);
		for (auto it2 = it1->second.tail_distance_trails_table.begin(); it2 != it1->second.tail_distance_trails_table.end(); it2++) {
			vertex_set.insert(it2->first);
		}
	}
	return vertex_set.size();
}

BestAnswer MultiStageGraph::get_best_single()const {
	BestAnswer ba;
	for (auto it1 = graphs[0].head_tail_distance_trails_table.begin(); it1 != graphs[0].head_tail_distance_trails_table.end(); it1++) {
		Graph total;
		for (auto it2 = it1->second.tail_distance_trails_table.begin(); it2 != it1->second.tail_distance_trails_table.end(); it2++) {
			WeightType w_min = it2->second.get_min_weight();
			total.insert(it1->first, it2->first, 0, CostType(w_min, 0));
		}
		for (int i = 1; i < graphs.size(); i++) {
			Graph temp;
			for (auto it3 = total.head_tail_distance_trails_table.begin(); it3 != total.head_tail_distance_trails_table.end(); it3++) {
				for (auto it4 = it3->second.tail_distance_trails_table.begin(); it4 != it3->second.tail_distance_trails_table.end(); it4++) {
					WeightType w_min_1 = it4->second.get_min_weight();
					auto it5 = graphs[i].head_tail_distance_trails_table.find(it4->first);
					if (it5 == graphs[i].head_tail_distance_trails_table.end()) continue;
					for (auto it6 = it5->second.tail_distance_trails_table.begin(); it6 != it5->second.tail_distance_trails_table.end(); it6++) {
						WeightType w_min_2 = it6->second.get_min_weight();
						temp.insert(it3->first, it6->first, 0, CostType(w_min_1 + w_min_2, 0));
					}
				}
			}
			std::swap(total, temp);
		}
		ba.insert(total.get_best());
	}
	return ba;
}

BestAnswer MultiStageGraph::get_best_cluster()const {
	BestAnswer ba;
	int count = 0;
	int percent = 0;
	int percent_former = percent;
	int total_count = graphs[0].head_tail_distance_trails_table.size();
	std::cout << "# total possible inputs = " << std::dec << total_count << "\n";
	for (auto it = graphs[0].head_tail_distance_trails_table.begin(); it != graphs[0].head_tail_distance_trails_table.end(); it++) {
		count++;
		percent = count * 100 / total_count;
		if (percent != percent_former) {
			std::cout << "searching " << std::dec << percent << "%\n";
			percent_former = percent;
		}
		Graph total;
		total.head_tail_distance_trails_table.insert(*it);
		for (int i = 1; i < graphs.size(); i++)	total.add(graphs[i]);
		ba.insert(total.get_best());
	}
	return ba;
}