#include "Xor.h"


void Xor::refreshGate()
{
	bool flag=false;
	if((Input[0]->stateCache==0 && Input[1]->stateCache!=0) || (Input[1]->stateCache==0 && Input[0]->stateCache!=0))
		flag=true;
	else
		flag=false;
	if(flag)
		Output->setState(1);
	else
		Output->setState(0);

}