#pragma once
#include"item.h"
class DFA//ʶ���ķ���ǰ׺����Ŀ���淶����
{
public:
	vector<Item_set> item_sets;//��Ŀ���淶��
	struct state//״̬�ṹ
	{
		map<token, int> go_to;//���״̬

	};
	vector<state> states;//״̬���ϣ�����Ŀһһ��Ӧ��
	int state_num;//״̬��
	void ini();//��ʼ������
};