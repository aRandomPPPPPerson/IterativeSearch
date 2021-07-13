#pragma once
#include <time.h>
#include <string>
using std::string;

class Timer {
	clock_t start;
	clock_t end;
public:
	void tik();
	void tok();
	double tiktok()const;
	string getCurrentTime()const;
};