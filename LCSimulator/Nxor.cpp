#include "Nxor.h"


void Nxor::refreshGate()
{
	bool flag=false;
	if((Input[0]->stateCache==0 && Input[1]->stateCache!=0) || (Input[1]->stateCache==0 && Input[0]->stateCache!=0))
		flag=true;
	else
		flag=false;
	if(flag)
		Output->setState(0);
	else
		Output->setState(1);
}