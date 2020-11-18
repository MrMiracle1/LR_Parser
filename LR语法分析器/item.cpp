#include"item.h"
#include<iostream>
Item_set::Item_set(set<Item> old):items(old)//��Ŀ�����캯��
{
	item_set_closure();//��������Ŀ�ıհ�
	for (auto item : items)//������Ŀ
	{
		int cur_production = item.production_num;
		if (item.point_position + 1 <=g.productions[cur_production].right.size())//��Ϊ��Լ���ƽ���Ŀ
		{
			token input_token = g.productions[cur_production].right[item.point_position];
			trans[input_token].insert(item);//����trans��
		}
		else
			reduce_items.insert(item);//��Ϊ��Լ��Ŀ����������Լ��Ŀ��
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
	while (new_items.size())//�����²���ʽ���������һ��ѭ��
	{
		set<Item> temp_items;
		for (auto current_item : new_items)//������һ��ѭ�������²���ʽ
		{
			production current_production = g.productions[current_item.production_num];
			if (current_item.point_position <current_production.right.size() && find(g.non_terminators.begin(), g.non_terminators.end(), current_production.right[current_item.point_position]) != g.non_terminators.end())//�Ǵ�Լ��Ŀ���ҵ��Ϊ���ս��
			{
				token current_token = current_production.right[current_item.point_position];
				for (auto i : g.productions_class[current_token])//�����÷��ս�����в���ʽ
				{
					Item new_item = { i,0 };//��λ�ó�ʼ����������ʱ��Ŀ���м������ʽ
					if(!in_item_set(new_item))
						temp_items.insert(new_item);
				}
			}
		}
		items.insert(temp_items.begin(), temp_items.end());//������Ŀ��
		new_items = temp_items;//��ѭ���²���ʽ
	}
}

void Item_set::output_item_set()//��ӡ��Ŀ��
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