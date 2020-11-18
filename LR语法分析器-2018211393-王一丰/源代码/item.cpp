#include"item.h"
#include<iostream>
Item_set::Item_set(set<Item> old):items(old)//项目集构造函数
{
	item_set_closure();//求现有项目的闭包
	for (auto item : items)//遍历项目
	{
		int cur_production = item.production_num;
		if (item.point_position + 1 <=g.productions[cur_production].right.size())//若为待约或移进项目
		{
			token input_token = g.productions[cur_production].right[item.point_position];
			trans[input_token].insert(item);//加入trans集
		}
		else
			reduce_items.insert(item);//若为规约项目，将其加入规约项目集
	}
}

bool Item_set::in_item_set(Item m)
{
	for (auto i : items)
	{
		if (m == i)
			return true;
	}
	return false;
}

void Item_set::item_set_closure()
{
	set<Item> new_items = items;
	while (new_items.size())//若有新产生式，则进入新一次循环
	{
		set<Item> temp_items;
		for (auto current_item : new_items)//遍历上一次循环所有新产生式
		{
			production current_production = g.productions[current_item.production_num];
			if (current_item.point_position <current_production.right.size() && find(g.non_terminators.begin(), g.non_terminators.end(), current_production.right[current_item.point_position]) != g.non_terminators.end())//是待约项目，且点后为非终结符
			{
				token current_token = current_production.right[current_item.point_position];
				for (auto i : g.productions_class[current_token])//遍历该非终结符所有产生式
				{
					Item new_item = { i,0 };//点位置初始化，并向临时项目集中加入产生式
					if(!in_item_set(new_item))
						temp_items.insert(new_item);
				}
			}
		}
		items.insert(temp_items.begin(), temp_items.end());//更新项目集
		new_items = temp_items;//本循环新产生式
	}
}

void Item_set::output_item_set()//打印项目集
{
	for (auto item : items)
	{
		production p = g.productions[item.production_num];
		cout << p.left << "->";
		int i;
		for (i = 0;i < p.right.size();i++)
		{
			if (i == item.point_position)
				cout << ".";
			cout << p.right[i];
		}
		if(i == item.point_position)
			cout << ".";
		cout << endl;
	}
	cout << endl;
}