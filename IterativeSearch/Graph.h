#pragma once
#include "CostType.h"
#include <map>
using std::map;
using std::make_pair;
#include <ostream>
using std::ostream;
#include "State.h"
#include <functional>
using std::function;

struct TrailCount {
private:
	map<CostType, int> trail_count;
public:
	TrailCount operator+(const TrailCount& rhs)const;
	TrailCount operator*(const TrailCount& rhs)const;
	void insert(CostType c, int n) { trail_count.insert(make_pair(c, n)); }
	friend ostream& operator<<(ostream& out, const TrailCount& obj);
};

struct Trails {
	CostType total_cost;
	TrailCount trails;
	Trails() {}
	Trails(CostType c, int n) :total_cost(c* n) { trails.insert(total_cost, n); }
	Trails(CostType c) :total_cost(c) {}
	Trails operator+(const Trails& rhs)const;
	Trails& operator+=(const Trails& rhs);
	Trails operator*(const Trails& rhs)const;
	Trails& operator*=(const Trails& rhs);
	bool operator<(const Trails& rhs)const;
	friend ostream& operator<<(ostream& out, const Trails& obj);
};

struct DistanceTrailsTable {
	map<int, Trails> distance_trails_table;
	DistanceTrailsTable() {}
	DistanceTrailsTable(int d, CostType c) { insert(d, c); }
	DistanceTrailsTable(int d, CostType c, int n) { insert(d, c, n); }
	DistanceTrailsTable(int d, Trails ts) { distance_trails_table.insert(make_pair(d, ts)); }
	void insert(int d, CostType c) { auto ret = distance_trails_table.insert(make_pair(d, c)); if (!ret.second)if (c.weight < ret.first->second.total_cost.weight)ret.first->second.total_cost = c; }
	void insert(int d, CostType c, int n) { auto ret = distance_trails_table.insert(make_pair(d, Trails(c, n))); }
	void add(int d, Trails ts);
	WeightType get_min_weight()const;
	friend ostream& operator<<(ostream& out, const DistanceTrailsTable& obj);
};

struct TailDistanceTrailsTable {
	map<State, DistanceTrailsTable> tail_distance_trails_table;
	TailDistanceTrailsTable() {}
	TailDistanceTrailsTable(const State& tail, int d, CostType c) { insert(tail, d, c); }
	TailDistanceTrailsTable(const State& tail, int d, CostType c, int n) { insert(tail, d, c, n); }
	TailDistanceTrailsTable(const State& tail, int d, Trails ti) { tail_distance_trails_table.insert(make_pair(tail, DistanceTrailsTable(d, ti))); }
	void insert(const State& sd, int d, CostType c) { auto ret = tail_distance_trails_table.insert(make_pair(sd, DistanceTrailsTable(d, c))); if (!ret.second)ret.first->second.insert(d, c); }
	void insert(const State& sd, int d, CostType c, int n) { auto ret = tail_distance_trails_table.insert(make_pair(sd, DistanceTrailsTable(d, c, n))); if (!ret.second)ret.first->second.insert(d, c, n); }
	void add(const State& sd, int d, Trails ts);
	friend ostream& operator<<(ostream& out, const TailDistanceTrailsTable& obj);
};

struct BestAnswer{
public:
	Trails trails;
	int distance;
	vector<State> state;
	void insert(const State& __input, const State& __output, int __d, const Trails& __ts);
	void insert(const BestAnswer& __ba);
	friend ostream& operator<<(ostream& out, const BestAnswer& obj);
};

struct HeadTailDistanceTrailsTable {
private:
	bool _reduce();
	//bool _reduce_head();
	//bool _reduce_tail();
public:
	map<State, TailDistanceTrailsTable> head_tail_distance_trails_table;
	HeadTailDistanceTrailsTable() {}
	HeadTailDistanceTrailsTable(const State& head, const State& tail, int d, CostType c) { insert(head, tail, d, c); }
	HeadTailDistanceTrailsTable(const State& head, const State& tail, int d, CostType c, int n) { insert(head, tail, d, c, n); }
	HeadTailDistanceTrailsTable(const State& head, const State& tail, int d, const Trails& ts) { head_tail_distance_trails_table.insert(make_pair(head, TailDistanceTrailsTable(tail, d, ts))); }
	void insert(const State& head, const State& tail, int d, CostType c) { auto ret = head_tail_distance_trails_table.insert(make_pair(head, TailDistanceTrailsTable(tail, d, c))); if (!ret.second)ret.first->second.insert(tail, d, c); }
	void insert(const State& head, const State& tail, int d, CostType c, int n) { auto ret = head_tail_distance_trails_table.insert(make_pair(head, TailDistanceTrailsTable(tail, d, c, n))); if (!ret.second)ret.first->second.insert(tail, d, c, n); }
	void add(const State& head, const State& tail, int d, Trails ts);

	int get_edge_num()const;
	int get_vertex_num()const;
	void reduce();

	void add(const HeadTailDistanceTrailsTable& rhs);
	void add(const HeadTailDistanceTrailsTable& rhs, const State& constant);

	WeightType get_min_weight(const State& head, const State& tail)const { return head_tail_distance_trails_table.find(head)->second.tail_distance_trails_table.find(tail)->second.get_min_weight(); }
	BestAnswer get_best()const;
	BestAnswer get_best_iterative()const;
	BestAnswer get_best(function<bool(const State&, const State&)> checker, function<State(const State&)> output_truncator, function<int(const State&)> to_repre)const;
	vector<WeightType> find_best_iterative(int r)const;

	friend ostream& operator<<(ostream& out, const HeadTailDistanceTrailsTable& obj);
};

using Graph = HeadTailDistanceTrailsTable;

struct MultiStageGraph {
	vector<Graph> graphs;
	void reduce();
	void prune(function<bool(const State&)> input_checker, function<bool(const State&)> output_checker);

	BestAnswer get_best_single()const;
	BestAnswer get_best_cluster()const;
	BestAnswer get_best_cluster(function<bool(const State&, const State&)> checker, function<State(const State&)> output_truncator, function<int(const State&)> to_repre)const;
	//BestAnswer get_best_cluster(function<State(const State&)> output_truncator, function<int(const State&)> to_repre)const;
	BestAnswer get_best_cluster(const vector<State>& c)const;
};
