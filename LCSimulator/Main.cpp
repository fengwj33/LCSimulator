#include<iostream>
#include<stdlib.h>
#include<string>
#include<vector>
#include<fstream>
#include"LCSimulator.h"
using namespace std;
int str2int(string str)
{
	int temp=0;
	for(int i=0;i<str.size();i++)
	{
		temp*=10;
		temp+=str[i]-'0';
	}
	return temp;
}
vector<string>  split(string str,char* delim)
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
int main()
{
	LCSimulator simulator;
	string Filename;
	string Testname;
    cout<<"Module:";
	cin>>Filename;
	cout<<"TestModule:";
	cin>>Testname;
	//simulator.LoadFile("C:\\Users\\Feng\\Desktop\\Add.txt");
	simulator.LoadFile((char *)Filename.c_str());
	vector<string>s_Split;
	vector<string>Listener;
	ifstream in(Testname.c_str());
	string statement;
	while(getline(in,statement))
	{
        cout<<">>"<<statement<<endl;
		//cin>>statement;
		s_Split=split(statement," ");
		if(s_Split[0].compare("AddListener")==0)
		{
			vector<string> temp;

			temp=split(s_Split[1],",");
			for(int i=0;i<temp.size();i++)
				Listener.push_back(temp[i]);
		}
		else if(s_Split[0].compare("GO")==0 || s_Split[0].compare("go")==0)
		{
		    cout<<"===========================begin==============================="<<endl;
			int time=simulator.Apply();
			for(int i=0;i<Listener.size();i++)
			{
				vector<string> l_Split;
				l_Split=split(Listener[i],":");
				if(l_Split.size()==1)
					cout<<Listener[i]<<":"<<simulator.getWireState(Listener[i])<<endl;
				else
				{
					string tempN="";
					for(int i=str2int(l_Split[1])-1;i>=0;i--)
					{
						tempN+=(char)(simulator.getWireState(l_Split[0]+":"+(char)(i+'0'))+'0');
					}
					cout<<l_Split[0]<<":"<<tempN<<endl;
				}
			}
			cout<<"time="<<time<<endl;
			cout<<"===========================end================================="<<endl;
		}
		else
		{
			vector<string> temp;
			temp=split(s_Split[0],"=");
			vector<string> n_Split;
			n_Split=split(temp[0],":");
			if(n_Split.size()==1)
            {
                if(temp[1].compare("0")==0)
                    simulator.setWireState(temp[0],0);
                else
                    simulator.setWireState(temp[0],1);
            }
            else
            {
                for(int i=0;i<str2int(n_Split[1]);i++)
                {
                    if(temp[1].c_str()[str2int(n_Split[1])-i-1]=='0')
                        simulator.setWireState(n_Split[0]+":"+(char)(i+'0'),0);
                    else
                        simulator.setWireState(n_Split[0]+":"+(char)(i+'0'),1);
                }
            }
		}
	}



	system("pause");
	return 0;
}
