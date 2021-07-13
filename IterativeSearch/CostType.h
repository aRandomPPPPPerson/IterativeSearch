#pragma once
#include <ostream>
typedef float WeightType;

const WeightType INF = 100000;

struct CostType {
	WeightType weight;
	bool sign;

	explicit CostType();
	explicit CostType(WeightType w, bool s);

	CostType& operator+=(const CostType& rhs);
	const CostType operator+(const CostType& rhs)const;
	CostType& operator*=(const CostType& rhs);
	const CostType operator*(const CostType& rhs)const;
	CostType& operator*=(int n);
	const CostType operator*(int n)const;

	bool operator<(const CostType& rhs)const;
	bool operator==(const CostType& rhs)const;
	bool operator>(const CostType& rhs)const;
	bool operator<=(const CostType& rhs)const;
	bool operator>=(const CostType& rhs)const;
	bool operator!=(const CostType& rhs)const;

	bool is_zero()const;

	CostType amplitude()const;

	friend std::ostream& operator<<(std::ostream& out, const CostType& obj);
};