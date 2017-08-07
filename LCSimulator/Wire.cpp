#include "Wire.h"


Wire::Wire(void)
{
	state=-1;
	stateCache=-1;
}
void Wire::setState(int State)
{
	state=State;
	stateCache=State;
}
bool Wire::swapstate()
{
    bool flag;
    if(stateCache==state)
        flag=false;
    else
        flag=true;
	stateCache=state;
	state=-1;
	return flag;
}
