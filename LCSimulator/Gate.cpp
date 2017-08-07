#include "Gate.h"


Gate::Gate()
{
	

}
void Gate::initGate(Wire* output, vector<Wire*> input)
{
	Output=output;
	Input.clear();
	for(int i=0;i<input.size();i++)
	{
		Input.push_back(input[i]);
	}
}
void Gate::refreshGate()
{
	Wire* temp;
	for(int i=0;i<Input.size();i++)
	{
		//temp=Input[i];
		//Output->state=-1;
	}
}


Gate::~Gate(void)
{
}
