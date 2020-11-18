#pragma once
#include"grammar.h"
#include"dfa.h"
enum action_type//����ö������
{
	shift,
	reduce,
	acc
};

struct action//��������
{
	action_type type;
	int value;
};


class parser //LR(1)�﷨��������
{
public:
	DFA dfa;//�÷���������DFA
	vector<map<token, action>> action_table;//action���
	vector<map<token, int>> goto_table;//goto���
	vector<token> token_stream;//�ַ���
	string action_name[3] = { "shift","reduce","acc" };//������������
public:
	void input_token();//�ַ�������
	void table_derivation();//Ԥ���������
	void output_parser_table();//��ӡ������
	void predict();//Ԥ���������
};