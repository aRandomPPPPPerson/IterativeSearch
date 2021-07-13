#include "Timer.h"
using std::to_string;

void Timer::tik() {
	start = clock();
}
void Timer::tok() {
	end = clock();
}
double Timer::tiktok()const {
	return (double)(end - start) / CLOCKS_PER_SEC;
}
string Timer::getCurrentTime()const {
	string s = "";
	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);
	s += "year: " + to_string(1900 + ltm.tm_year) + "\t";
	s += "month: " + to_string(1 + ltm.tm_mon) + "\t";
	s += "day: " + to_string(ltm.tm_mday) + "\t";
	s += "time: " + to_string(ltm.tm_hour) + ":";
	s += to_string(ltm.tm_min) + ":";
	s += to_string(ltm.tm_sec);
	return s;
}