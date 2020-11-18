#pragma once
#include<set>
#include<map>
#include"grammar.h"

using namespace std;

typedef string token;

struct Item//项目类
{
	int production_num;//产生式编号
	int point_position;//点的位置
	//运算符的重载
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
	
class Item_set//项目集类
{
public:
	set<Item> items;//项目集合
	map<token, set<Item>> trans;//后继项目集合
	set<Item> reduce_items;//规约项目集合
public:
	Item_set(set<Item>);//通过初始项目集构造新项目集
	void item_set_closure();//闭包运算函数
	bool in_item_set(Item);//判断项目是否存在于项目集
	void output_item_set();//输出项目集
	bool operator ==(const Item_set& rhs) const {//运算符重载
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