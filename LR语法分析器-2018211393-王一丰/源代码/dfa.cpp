#include"dfa.h"
#include<queue>
#include<iostream>
void DFA::ini()//创建识别活前缀的DFA及项目集规范族
{
	g.extend();//拓广文法
	g.ini_productions_class();//初始化productions_class
	cout << "拓广文法：" << endl;
	g.output_productions();//打印拓广文法产生式
	//开始项目集初始化
	cout << "LR(0)项目集规范族：" << endl;
	Item t = { 0,0 };
	set<Item> t_set;
	t_set.insert(t);
	Item_set start_item_set(t_set);
	item_sets.push_back(start_item_set);

	cout << "I" << item_sets.size()-1 << ":" << endl;//打印首项目集
	start_item_set.output_item_set();

	states.push_back({});
	queue<int> q;
	q.push(0);
	while (q.size())//遍历广度分析队列
	{
		int cur_item_set = q.front();//目前分析项目集
		q.pop();
		//for (pair<token, set<Item>> token_item : item_sets[cur_item_set].trans)//打印其后续项目
		//{
		//	cout << token_item.first<<":";
		//	for (auto item : token_item.second)
		//		cout << item.production_num << " ";
		//	cout << endl;
		//}
		map<token, set<Item>> temp;
		for (auto iter = item_sets[cur_item_set].trans.begin();iter != item_sets[cur_item_set].trans.end();iter++)
		{
			temp.insert(*iter);
		}
		for (auto iter = temp.begin();iter != temp.end();iter++)//遍历后续项目集
		{
			auto token_item = *iter;
			token input_token = token_item.first;
			set<Item> next_items =token_item.second;
			set<Item> new_items;
			for (auto item : next_items)//遍历项目集中现有项目
			{
				Item new_item = item;
				new_item.point_position++;
				new_items.insert(new_item);
			}
			Item_set new_item_set(new_items);//更新项目集
			auto set_num = find(item_sets.begin(), item_sets.end(), new_item_set);
			if (set_num == item_sets.end())//若项目集规范组中无此项目集，将其加入项目集
			{
				item_sets.push_back(new_item_set);
				states[cur_item_set].go_to[input_token] = item_sets.size() - 1;

				cout << "I" << item_sets.size()-1 << ":" << endl;
				new_item_set.output_item_set();//打印新项目集

				q.push(item_sets.size()-1);
				states.push_back({});
			}
			else
			{
				states[cur_item_set].go_to[input_token] = set_num-item_sets.begin();
			}
		}
	}
	state_num = states.size();
}