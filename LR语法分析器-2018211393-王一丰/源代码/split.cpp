#include"split.h"

using namespace std;
 
vector<string> split(const string &str,char ch)//�ַ����ָ����� 
{
	vector<string> v;
	string s;
	for(auto c:str)
	{
		if(c!=ch)
		{
			s+=c;
		}
		else
		{
			v.push_back(s);
			s="";
		}
	}
	v.push_back(s);
	return v;
}

