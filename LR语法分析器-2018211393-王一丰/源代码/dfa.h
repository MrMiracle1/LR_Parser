#pragma once
#include"item.h"
class DFA//识别文法活前缀的项目集规范族类
{
public:
	vector<Item_set> item_sets;//项目集规范族
	struct state//状态结构
	{
		map<token, int> go_to;//后继状态

	};
	vector<state> states;//状态集合（与项目一一对应）
	int state_num;//状态数
	void ini();//初始化函数
};