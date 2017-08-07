#pragma once
#include<vector>
#include"Wire.h"
using namespace std;
class Gate
{
public:
	vector<Wire*> Input;
	Wire* Output;
	Gate();
	void initGate(Wire* output, vector<Wire*> input);
	virtual void refreshGate();
	~Gate(void);
};

