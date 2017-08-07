#pragma once
#include<iostream>
using namespace std;
class Wire
{
public:
	string WireName;
	int state;
	int stateCache;
	void setState(int State);
	bool swapstate();
	Wire(void);

};

