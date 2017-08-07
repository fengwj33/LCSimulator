#include "LCSimulator.h"

string& trimf(string &str,string i)
{
    if (str.empty())
        return str;
	str.erase(0,str.find_first_not_of(i));
    return str;
}
string& triml(string &str,string i)
{
    if (str.empty())
        return str;
	str.erase(str.find_last_not_of(i) + 1);
    return str;
}

C_stack::C_stack()
{
	stack.clear();
	mappedStack.clear();
}
bool C_stack::isNull()
{
	if(stack.size()==0)
		return true;
	else
		return false;
}
void C_stack::push(string value,MappedList mapped)
{
	stack.push_back(value);
	mappedStack.push_back(mapped);
}
string C_stack::pop()
{
	string temp=stack[stack.size()-1];
	stack.pop_back();
	mappedStack.pop_back();
	return temp;
}
string C_stack::getAddr()
{
	string temp;
	for(int i=0;i<stack.size();i++)
	{
		temp+=stack[i];
		temp+="-";
	}
	return temp;
}
MappedList C_stack::getMapped()
{
	MappedList temp=mappedStack[stack.size()-1];
	return temp;
}





LCSimulator::LCSimulator(void)
{
	WireList.clear();
	GateList.clear();
	WireStates.clear();
	InputAutoClear=false;
}


LCSimulator::~LCSimulator(void)
{
}
void LCSimulator::addWire(string name)
{
	vector<string> n_Split;
	n_Split=split(name,":");
	name=completingWireName(n_Split[0]);
	if(n_Split.size()==1)
	{
		Wire* temp=new Wire();
		temp->WireName=name;
		WireList.push_back(temp);
		WireStates.push_back(0);
	}
	else
	{
		for(int i=0;i<str2int(n_Split[1]);i++)
		{
			Wire* temp=new Wire();
			temp->WireName=name+":"+(char)(i+'0');
			WireList.push_back(temp);
			WireStates.push_back(0);
		}
	}
}
void LCSimulator::addGate(string type,string output,vector<string>input)
{
	Gate* temp=newGate(type);
	vector<Wire*> inputlist;
	inputlist.clear();
	for(int i=0;i<input.size();i++)
		inputlist.push_back(findWire(input[i]));
	temp->initGate(findWire(output),inputlist);
	GateList.push_back(temp);
}
int LCSimulator::getWireState(string name)
{
	Wire* temp=findWire(name);
	return temp->state;
}
void LCSimulator::setWireState(string name,int state)
{
	int id=findWireID(name);
	bool found;
	found=false;
	for(int i=0;i<LogicInputList.size();i++)
    {
        if(LogicInputList[i].WireID==id)
        {
            found=true;
            LogicInputList[i].State=state;
            break;
        }
    }
	if(!found)
    {
        InputStruct temp;
        temp.WireID=id;
        temp.State=state;
        LogicInputList.push_back(temp);
    }
}
int LCSimulator::Apply()
{
	Gate* temp;
	bool flag;
	int time;
	time=0;
	while(1)
	{
	    time++;
		applyLogicInput();
		StatesBackup();
		flag=false;
		for(int j=0;j<WireList.size();j++)
		{
			WireList[j]->swapstate();
		}
		for(int j=0;j<GateList.size();j++)
		{
			temp=GateList[j];
			temp->refreshGate();
		}
		applyLogicInput();
		flag=CheckStates();
		if(flag)
            break;
	}
	if(InputAutoClear)
        LogicInputList.clear();
    return time;
}

Gate* LCSimulator::newGate(string type)
{
	Gate* temp;
	if(type.compare("and")==0)
		temp=new And();
	else if(type.compare("nand")==0)
		temp=new Nand();
	else if(type.compare("or")==0)
		temp=new Or();
	else if(type.compare("nor")==0)
		temp=new Nor();
	else if(type.compare("xor")==0)
		temp=new Xor();
	else if(type.compare("nxor")==0)
		temp=new Nxor();
	return temp;
}
Wire* LCSimulator::findWire(string name)
{

	Wire* temp;
	int id;
	id=findWireID(name);
	if(id==-1)
		return (Wire*)0;
	else
		return WireList[id];
}
int LCSimulator::findWireID(string name)
{
	name=completingWireName(name);
	Wire* temp;
	for(int i=0;i<WireList.size();i++)
	{
		temp=WireList[i];
		if(temp->WireName.compare(name)==0)
		{
			return i;
		}
	}
	cout<<"cannot found "<<name<<endl;
	return -1;
}
void LCSimulator::applyLogicInput()
{
	InputStruct temp;
	Wire* wtemp;
	for(int i=0;i<LogicInputList.size();i++)
	{
		temp=LogicInputList[i];
		wtemp=WireList[temp.WireID];
		wtemp->state=temp.State;
	}
}
void LCSimulator::AnalyzeLine(string statement)
{
	trimf(statement,"\t");
	trimf(statement," ");
	if(statement=="") return;
	vector<string> s_Split;
	s_Split=split(statement," ");
	if(s_Split[0].compare("Wire")==0)
	{
		for(int i=1;i<s_Split.size();i++)
		{
			addWire(s_Split[i]);
		}
	}
	else if(s_Split[0].compare("AddGate")==0)
	{
		vector<string> temp;
		temp.clear();
		for(int i=3;i<s_Split.size();i++)
		{
			temp.push_back(s_Split[i]);
		}
		addGate(s_Split[1],s_Split[2],temp);
	}
	else if(s_Split[0].compare("AddModule")==0)
	{
		Module module;
		module=findModule(s_Split[1]);
		vector<string> interfacelist;
		interfacelist.clear();

		if(s_Split.size()>=4)
		{
			vector<string> i_Split;
			i_Split=split(s_Split[3],",");
			for(int j=0;j<i_Split.size();j++)
			{
				if(i_Split[j].compare("NULL")!=0)
					i_Split[j]=completingWireName(i_Split[j]);
				interfacelist.push_back(i_Split[j]);
			}
		}

		constructModule(module,s_Split[2],interfacelist);
	}
}

bool LCSimulator::LoadFile(char* filename)
{
	//string FileName;
	//FileName=filename;
	script.clear();
	modules.clear();
	ifstream in(filename);
	if(in)
	{
		string s;
		while(getline(in,s))
			script.push_back(s);
			//AnalyzeLine(s);
		in.close();
	}
	else
	{
		cout<<"error";
		return false;
	}
	int AnyState=0;
	Module temp;
	//----------------------------------------------------拆分模块
	for(int i=0;i<script.size();i++)
	{

		trimf(script[i],"\t");
		trimf(script[i]," ");
		vector<string> s_Split;
		s_Split=split(script[i]," ");
		if(s_Split[0].compare("MainModule")==0)
		{
			mainModule=s_Split[1];
		}
		if(AnyState==0)
		{
			if(s_Split[0].compare("Module")==0)
			{
				temp.Name=s_Split[1];
				temp.from=i+1;
				vector<string> interfacelist;
				interfacelist.clear();
				if(s_Split.size()>2)
				{
					interfacelist=split(s_Split[2],",");
				}
				temp.WireInterface=interfacelist;
				AnyState=1;
			}
		}
		else
		{
			if(s_Split[0].compare("EndModule")==0)
			{
				AnyState=0;
				temp.to=i-1;
				modules.push_back(temp);
			}
		}
	}
	/*
	cout<<mainModule<<endl;
	for(int i=0;i<modules.size();i++)
	{
		cout<<modules[i].Name<<endl;
		for(int j=0;j<modules[i].WireInterface.size();j++)
		{
			cout<<modules[i].WireInterface[j]<<endl;
		}
		cout<<"from:"<<modules[i].from<<endl;
		cout<<"to:"<<modules[i].to<<endl<<endl;
	}
	*/
	//------------------------------------------构建逻辑电路
	Module mainm;
	mainm=findModule(mainModule);
	vector<string> empvector;
	empvector.clear();
	constructModule(mainm,"Main",empvector);


	while(1)
	{
		if(conStack.isNull()) break;
		conStack.pop();
	}
	MappedList emptylist;
	conStack.push(mainm.Name+":Main",emptylist);
		//AnalyzeLine(script[i]);

	/*
	for(int i=0;i<WireList.size();i++)
	{
		cout<<i<<":"<<WireList[i]->WireName<<endl;

	}
	for(int i=0;i<GateList.size();i++)
	{
		Gate tempG;
		tempG=*GateList[i];
		cout<<"out:"<<endl;
		cout<<tempG.Output->WireName<<endl;
		cout<<"in:"<<endl;
		for(int j=0;j<tempG.Input.size();j++)
		{
			cout<<tempG.Input[j]->WireName<<endl;
		}

	}*/

}
void LCSimulator::constructModule(Module module,string description,vector<string> interfaces)
{
	MappedList temp;
	temp.wiremapped.clear();
	WireMapped wmtemp;
	for(int i=0;i<module.WireInterface.size();i++)
	{
		if(interfaces[i].compare("NULL")!=0)
		{
			wmtemp.OrigName=module.WireInterface[i];
			wmtemp.FullName=interfaces[i];
			temp.wiremapped.push_back(wmtemp);
		}
	}
	conStack.push(module.Name+":"+description,temp);
	for(int i=0;i<module.WireInterface.size();i++)
	{
		if(interfaces[i].compare("NULL")==0)
		{
			addWire(module.WireInterface[i]);
		}
	}
	for(int i=module.from;i<=module.to;i++)
	{
		AnalyzeLine(script[i]);
	}



	conStack.pop();
}
Module LCSimulator::findModule(string name)
{
	Module cnf;
	cnf.from=-1;
	for(int i=0;i<modules.size();i++)
	{
		if(modules[i].Name==name)
			return modules[i];
	}
	return cnf;
}
vector<string>  LCSimulator::split(string str,char* delim)
{
	char stcpy[255];
	vector<string> subst;
	char* temp;
	for(int i=0;i<str.size()+1;i++)
	{
		stcpy[i]=str.c_str()[i];
	}
	temp=strtok(stcpy,delim);
	while(true)
	{
		string stemp;
		stemp=temp;
		subst.push_back(stemp);
		temp=strtok(NULL,delim);
		if(temp==NULL)
			break;
	}
	return subst;
}

string LCSimulator::completingWireName(string WireName)
{
	MappedList temp;
	temp=conStack.getMapped();
	for(int i=0;i<temp.wiremapped.size();i++)
	{
		if(WireName.compare(temp.wiremapped[i].OrigName)==0)
			return temp.wiremapped[i].FullName;
	}
	return conStack.getAddr()+WireName;
}
int LCSimulator::str2int(string str)
{
	int temp=0;
	for(int i=0;i<str.size();i++)
	{
		temp*=10;
		temp+=str[i]-'0';
	}
	return temp;
}
void LCSimulator::StatesBackup()
{
    for(int i=0;i<WireStates.size();i++)
    {
        WireStates[i]=WireList[i]->state;
    }
}
bool LCSimulator::CheckStates()
{
    bool temp=true;
    for(int i=0;i<WireStates.size();i++)
    {
        if(WireStates[i]!=WireList[i]->state)
            temp=false;
    }
    return temp;
}
