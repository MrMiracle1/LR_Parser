#pragma once
#include<set>
#include<map>
#include"grammar.h"

using namespace std;

typedef string token;

struct Item//��Ŀ��
{
	int production_num;//����ʽ���
	int point_position;//���λ��
	//�����������
	bool operator <(const Item& rhs) const {
		return (production_num < rhs.production_num)
			|| (production_num == rhs.production_num && point_position < rhs.point_position);

	};
	bool operator ==(const Item& rhs) const {
		return (production_num == rhs.production_num
			&& point_position == rhs.point_position);
	};
	bool operator !=(const Item& rhs) const {
		return !(*this == rhs);
	};
};
	
class Item_set//��Ŀ����
{
public:
	set<Item> items;//��Ŀ����
	map<token, set<Item>> trans;//�����Ŀ����
	set<Item> reduce_items;//��Լ��Ŀ����
public:
	Item_set(set<Item>);//ͨ����ʼ��Ŀ����������Ŀ��
	void item_set_closure();//�հ����㺯��
	bool in_item_set(Item);//�ж���Ŀ�Ƿ��������Ŀ��
	void output_item_set();//�����Ŀ��
	bool operator ==(const Item_set& rhs) const {//���������
		if (items.size() == rhs.items.size()) {
			for (auto it(items.begin()), r_it(rhs.items.begin());it != items.end();++it, ++r_it) {
				if (*it != *r_it) {
					return false;
				}
			}
			return true;
		}
		return false;
	}
};