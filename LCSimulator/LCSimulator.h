#pragma once
#include"Wire.h"
#include"Gate.h"
#include"And.h"
#include"Nand.h"
#include"Or.h"
#include"Nor.h"
#include"Xor.h"
#include"Nxor.h"
#include<vector>
#include<fstream>
#include<string>
#include<cstring>
using namespace std;
struct InputStruct
{
	int WireID;
	int State;
};
struct WireMapped
{
	string OrigName;
	string FullName;
};
struct MappedList
{
	vector<WireMapped> wiremapped;
};
struct Module
{
	string Name;
	vector<string> WireInterface;
	int from;
	int to;
};
class C_stack
{
public:
	C_stack(void);
	bool isNull();
	void push(string value,MappedList mapped);
	string pop();
	string getAddr();
	MappedList getMapped();
private:
	vector<string> stack;
	vector<MappedList> mappedStack;
};

class LCSimulator
{
public:
	LCSimulator(void);
	~LCSimulator(void);
	vector<Wire*> WireList;
	vector<Gate*> GateList;
	vector<InputStruct> LogicInputList;
	void addWire(string name);
	void addGate(string type,string output,vector<string>input);
	int getWireState(string name);
	void setWireState(string name,int state);
	void AnalyzeLine(string statement);
	bool LoadFile(char* filename);
	int Apply();
	bool InputAutoClear;
private:
	Gate* newGate(string type);
	Wire* findWire(string name);
	int findWireID(string name);
	void applyLogicInput();
	static vector<string> split(string str,char* delim);
	C_stack conStack;
	vector<string> script;
	vector<Module> modules;
	string mainModule;
	void constructModule(Module module,string description,vector<string> interfaces);
	Module findModule(string name);
	string completingWireName(string WireName);
	int str2int(string str);
	vector<int>WireStates;
	void StatesBackup();
	bool CheckStates();
};
