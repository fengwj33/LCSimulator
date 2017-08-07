#include "Nand.h"
void Nand::refreshGate()
{
	Wire* temp;
	bool flag=true;
	for(int i=0;i<Input.size();i++)
	{
		temp=Input[i];
		if(temp->stateCache==0)
			flag=false;
		//Output->state=-1;
	}
	if(flag)
	{
		Output->setState(0);
	}
	else
	{
		Output->setState(1);
	}
	

}
