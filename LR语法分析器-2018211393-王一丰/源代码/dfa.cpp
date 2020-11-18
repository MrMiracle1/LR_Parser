#include"dfa.h"
#include<queue>
#include<iostream>
void DFA::ini()//����ʶ���ǰ׺��DFA����Ŀ���淶��
{
	g.extend();//�ع��ķ�
	g.ini_productions_class();//��ʼ��productions_class
	cout << "�ع��ķ���" << endl;
	g.output_productions();//��ӡ�ع��ķ�����ʽ
	//��ʼ��Ŀ����ʼ��
	cout << "LR(0)��Ŀ���淶�壺" << endl;
	Item t = { 0,0 };
	set<Item> t_set;
	t_set.insert(t);
	Item_set start_item_set(t_set);
	item_sets.push_back(start_item_set);

	cout << "I" << item_sets.size()-1 << ":" << endl;//��ӡ����Ŀ��
	start_item_set.output_item_set();

	states.push_back({});
	queue<int> q;
	q.push(0);
	while (q.size())//������ȷ�������
	{
		int cur_item_set = q.front();//Ŀǰ������Ŀ��
		q.pop();
		//for (pair<token, set<Item>> token_item : item_sets[cur_item_set].trans)//��ӡ�������Ŀ
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
		for (auto iter = temp.begin();iter != temp.end();iter++)//����������Ŀ��
		{
			auto token_item = *iter;
			token input_token = token_item.first;
			set<Item> next_items =token_item.second;
			set<Item> new_items;
			for (auto item : next_items)//������Ŀ����������Ŀ
			{
				Item new_item = item;
				new_item.point_position++;
				new_items.insert(new_item);
			}
			Item_set new_item_set(new_items);//������Ŀ��
			auto set_num = find(item_sets.begin(), item_sets.end(), new_item_set);
			if (set_num == item_sets.end())//����Ŀ���淶�����޴���Ŀ�������������Ŀ��
			{
				item_sets.push_back(new_item_set);
				states[cur_item_set].go_to[input_token] = item_sets.size() - 1;

				cout << "I" << item_sets.size()-1 << ":" << endl;
				new_item_set.output_item_set();//��ӡ����Ŀ��

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