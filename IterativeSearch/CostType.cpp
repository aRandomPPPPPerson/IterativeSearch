#include "CostType.h"
#include <math.h>

CostType::CostType() :weight(INF), sign(0) {}
CostType::CostType(WeightType w, bool s) : weight(w), sign(s) {}

CostType& CostType::operator+=(const CostType& rhs) {
	if (rhs.weight == INF) {

	}
	else if (weight == INF) {
		*this = rhs;
	}
	else if (sign != rhs.sign) {
		if (weight == rhs.weight) {
			weight = INF;
			sign = 0;
		}
		else if (weight < rhs.weight) {
			weight = weight - log(1.0 - pow(2, weight - rhs.weight)) / log(2.0);
			// sign = sign;
		}
		else {
			weight = rhs.weight - log(1.0 - pow(2, rhs.weight - weight)) / log(2.0);
			sign = rhs.sign;
		}
	}
	else { // if(sign == rhs.sign)
		weight = weight - log(1.0 + pow(2, weight - rhs.weight)) / log(2.0);
	}
	return *this;
}

const CostType CostType::operator+(const CostType& rhs)const {
	return CostType(*this) += rhs;
}

const CostType CostType::operator*(const CostType& rhs)const {
	return CostType(*this) *= rhs;
}

CostType& CostType::operator*=(const CostType& rhs) {
	if (weight == INF) {

	}
	else if (rhs.weight == INF) {
		weight = INF;
		sign = 0;
	}
	else {
		weight += rhs.weight;
		sign ^= rhs.sign;
	}
	return *this;
}

CostType& CostType::operator*=(int n) {
	if (weight == INF) {

	}
	else {
		weight -= log((float)n) / log(2.0);
	}
	return *this;
}

const CostType CostType::operator*(int n)const {
	return CostType(*this) *= n;
}

bool CostType::operator<(const CostType& rhs)const {
	if (sign > rhs.sign) return true;
	else if (sign < rhs.sign) return false;
	else return sign == 0 ? weight > rhs.weight:weight < rhs.weight;
}

bool CostType::operator==(const CostType& rhs)const {
	return (weight == rhs.weight && sign == rhs.sign);
}

bool CostType::operator>(const CostType& rhs)const {
	return !(*this < rhs) && !(*this == rhs);
}

bool CostType::operator<=(const CostType& rhs)const {
	return *this < rhs || *this == rhs;
}

bool CostType::operator>=(const CostType& rhs)const {
	return !(*this < rhs);
}

bool CostType::operator!=(const CostType& rhs)const {
	return !(*this == rhs);
}

bool CostType::is_zero()const {
	return weight == INF;
}

CostType CostType::amplitude()const {
	return CostType(this->weight, 0);
}

std::ostream& operator<<(std::ostream& out, const CostType& obj) {
	if (obj.weight == 0) {
		if (obj.sign == 1)
			out << -1;
		else
			out << 1;
	}
	else if (obj.weight == INF) {
		out << 0;
	}
	else {
		if (obj.sign == 1)
			out << "-";
		out << "2^{-" << std::dec << obj.weight << "}";
	}
	return out;
}