#pragma once
#include"grammar.h"
#include"dfa.h"
enum action_type//动作枚举类型
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
	DFA dfa;//该分析器依据DFA
	vector<map<token, action>> action_table;//action表格
	vector<map<token, int>> goto_table;//goto表格
	vector<token> token_stream;//字符流
	string action_name[3] = { "shift","reduce","acc" };//动作名称数组
public:
	void input_token();//字符流导入
	void table_derivation();//预测分析表创建
	void output_parser_table();//打印分析表
	void predict();//预测分析程序
};