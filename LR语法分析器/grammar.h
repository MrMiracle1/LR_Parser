#pragma once
#include<set>
#include<vector>
#include<map>
#include<deque>
#include<string>

using namespace std;

typedef string token;//符号类型

struct production {		//产生式数据结构
	token left;	//左侧非终结符
	deque<token> right;	//右侧产生式
};

class grammar {		//文法类
public:
	vector<token> non_terminators, terminators;		//非终结符集，终结符集
	token start_token;		//文法起始符
	vector<production> productions;		//文法所有产生式
	map<token, bool> first_over, follow_over,epsilon_over;		//记录非终结符first、follow集是否已求
	map<token, set<token>> first,follow;		//非终结符的first、follow集
	map<token, bool> epsilon;
	map<token, set<int>> productions_class;			//按产生式左侧非终结符为产生式分类
	vector<set<token>> first_of_production;		//产生式的first集
	map<token, map<token, bool>> follow_include;		//follow集的互相包含关系
public:
	grammar();		//默认构造函数，通过文件读入文法
	void infer_epsilon(const token&);//推导可推空符号
	void output_epsilon();//计算可推空非终结符
	void first_derivation(const token&);//对应非终结符first推导
	void follow_derivation(const token&);//对应非终结符follow堆推导
	void output_productions();//输出所有产生式
	void output_first();//计算first集
	void output_follow();//计算follow集
	void output_first_follow();//打印first、follow表
	void extend(); //拓广文法
	void ini_productions_class();//初始化productions_class
};

extern grammar g;//全局文法
