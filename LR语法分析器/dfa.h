#pragma once
#include"item.h"
class DFA
{
public:
	vector<Item_set> item_sets;
	struct state
	{
		map<token, int> go_to;

	};
	vector<state> states;
	int state_num;
	void ini();
};