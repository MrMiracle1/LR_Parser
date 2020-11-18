#include"parser.h"
#include<iostream>
#include<deque>
#include <iomanip>

void parser::input_token()//字符流读入
{
	FILE *fptr;
	fptr = fopen("token_stream.in", "r");
	string num="";
	while (!feof(fptr))
	{
		char c = fgetc(fptr);
		if (c == -1)
		{
			if (num.size())
			{
				token_stream.emplace_back("n");
				num = "";
			}
			break;
		}		
		if (c > '9' || c < '0')
		{
			if (num.size())
			{
				token_stream.emplace_back("n");
				num = "";
			}
			token t(1,c);
			token_stream.emplace_back(t);
		}
		else
		{
			num += c;
		}
	}
	token_stream.emplace_back("$");
	
}

void parser::table_derivation()//预测分析表
{
	cout << "first_follow集：" << endl;
	g.output_first_follow();//计算first_follow集
	action_table.resize(dfa.state_num);
	goto_table.resize(dfa.state_num);
	//打印state转换
	//for (int i = 0;i < dfa.state_num;i++)
	//{
	//	cout << i << ":";
	//	for (auto tran : dfa.states[i].go_to)
	//	{
	//		cout << tran.first << "," << tran.second;
	//	}
	//	cout << endl;
	//}

	for (int i=0 ;i< dfa.state_num;i++)
	{
		for (auto tran: dfa.states[i].go_to)
		{
			token cur_token = tran.first;
			if (find(g.terminators.begin(), g.terminators.end(), cur_token) != g.terminators.end())
				action_table[i][cur_token] = { shift,tran.second };
			else
				goto_table[i][cur_token] = { tran.second };
		}
		for (auto item : dfa.item_sets[i].reduce_items)
		{
			production current_production = g.productions[item.production_num];
			token cur_token = current_production.left;
			if(cur_token==g.start_token)
				action_table[i]["$"] = { acc,0 };
			else
			{
				for (auto t : g.follow[cur_token])
				{
					if (action_table[i].find(t) != action_table[i].end())
					{
						cout << "SLR(1)表项发生冲突" << endl;
						exit(0);
					}
					else
						action_table[i][t] = { reduce,item.production_num };
				}				
			}
		}
	}

}

void parser::output_parser_table()
{
	cout << "action表" << endl;
	cout << "-------------------------------------------------------------------------------------------" << endl;
	cout << setiosflags(ios::left) << setw(10)<<" ";
	g.terminators.erase(find(g.terminators.begin(), g.terminators.end(), "ε"));
	for (token i : g.terminators)
	{
		cout << setw(10) <<i; 
	}
	cout << endl;
	cout << "-------------------------------------------------------------------------------------------" << endl;
	for (int i=0 ; i<dfa.state_num;i++)
	{
		cout << setw(10) <<i;
		for (token j : g.terminators)
		{
			if (action_table[i].find(j) != action_table[i].end())
			{
				string s = "";
				s=action_name[action_table[i][j].type];
				if (action_table[i][j].type != acc)
					s += to_string(action_table[i][j].value);
				cout<< setw(10)<<s;
			}
			else
				cout << setw(10) << "error" ;
		}
		cout << endl;
	}
	cout << "goto表" << endl;
	cout << "-------------------------------------------------------------------------------------------" << endl;
	cout << setiosflags(ios::left) << setw(10) << " ";
	g.non_terminators.erase(find(g.non_terminators.begin(), g.non_terminators.end(), g.start_token));
	for (token i : g.non_terminators)
	{
		cout << setw(10) << i;
	}
	cout << endl;
	cout << "-------------------------------------------------------------------------------------------" << endl;
	for (int i = 0; i < dfa.state_num;i++)
	{
		cout << setw(10) << i;
		for (token j : g.non_terminators)
		{
			if (goto_table[i].find(j) != goto_table[i].end())
			{
				cout << setw(10) <<goto_table[i][j];
			}
			else
				cout << setw(10) << "error" ;
		}
		cout << endl;
	}
}

void parser::predict()//预测分析函数
{
	//for (auto i : token_stream)
	//	cout << i;
	//cout << endl;
	int step = 0;
	deque<pair<int,token>> parser_stack;
	parser_stack.push_back({ 0,"_" });
	cout << endl << "预测过程：" << endl;
	cout << "-----------------------------------------------" << endl;
	cout << setiosflags(ios::left) << setw(10)<<"步骤"<<setw(30) << "状态栈"<<setw(30) << "字符栈"
		<< setw(40) <<"输入" << setw(10) << "动作"<<endl;
	cout << "-----------------------------------------------" << endl;
	vector<token>::iterator iter = token_stream.begin();
	for (int step =1;iter != token_stream.end();step++)//遍历输入字符流
	{
		cout << setw(10) << step;
		//分析栈
		string s = "";
		for (int i=0;i<parser_stack.size();i++)
		{
			s+=to_string(parser_stack[i].first);
		}
		cout << setw(30)<<s;
		s = "";
		for (int i = 0;i < parser_stack.size();i++)
		{
			s += parser_stack[i].second;
		}
		cout << setw(30) << s;
		//打印剩余输入字符流
		vector<token>::iterator iter2 = iter;
		s = "";
		for (;iter2 != token_stream.end();iter2++)
		{
			s+=*iter2;
		}
		cout << setw(40)<<s;
		//打印输出
		s = "";
		pair<int, token> stack_item = parser_stack.back();
		action cur_action = action_table[stack_item.first][*iter];
		if (cur_action.type == shift)
		{
			s += action_name[shift];
			s += to_string(cur_action.value);
			parser_stack.push_back({ cur_action.value,*iter });
			iter++;
		}
		else if (cur_action.type == reduce)
		{
			s += action_name[reduce]+":";
			production p = g.productions[cur_action.value];
			s += p.left+"->";
			for (token j : p.right)
			{
				s += j;
				parser_stack.pop_back();
			}	
			int new_value = goto_table[parser_stack.back().first][p.left];
			parser_stack.push_back({ new_value,p.left });
		}
		else if (cur_action.type = acc)
		{
			cout << setw(10) << "acc" << endl;
			exit(0);
		}
		else
		{
			cout << setw(10) << "error" << endl;
			exit(0);
		}
		cout <<setw(10)<< s << endl;
	}
}