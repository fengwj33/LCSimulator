#include "Or.h"


void Or::refreshGate()
{
	Wire* temp;
	bool flag=false;
	for(int i=0;i<Input.size();i++)
	{
		temp=Input[i];
		if(temp->stateCache==1 || temp->stateCache==-1)
			flag=true;
		//Output->state=-1;
	}
	if(flag)
		Output->setState(1);
	else
		Output->setState(0);

}