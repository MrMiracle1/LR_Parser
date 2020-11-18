#pragma once
#include"grammar.h"
#include"dfa.h"
enum action_type
{
	shift,
	reduce,
	acc
};

struct action//动作表项
{
	action_type type;
	int value;
};


class parser //LR(1)语法分析器类
{
public:
	DFA dfa;
	vector<map<token, action>> action_table;
	vector<map<token, int>> goto_table;
	vector<token> token_stream;
	string action_name[3] = { "shift","reduce","acc" };
public:
	void input_token();
	void table_derivation();
	void output_parser_table();
	void predict();
};