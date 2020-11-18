#pragma once
#include<set>
#include<map>
#include"grammar.h"

using namespace std;

typedef string token;

struct Item
{
	int production_num;
	int point_position;
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
	
class Item_set
{
public:
	set<Item> items;
	map<token, set<Item>> trans;
	set<Item> reduce_items;
public:
	Item_set(set<Item>);
	void item_set_closure();
	bool in_item_set(Item);
	void output_item_set();
	bool operator ==(const Item_set& rhs) const {
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