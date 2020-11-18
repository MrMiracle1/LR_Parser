#include"grammar.h"
#include<iostream>
#include<algorithm>
#include<iomanip>
#include"split.h"

grammar g = grammar();

void grammar::extend()
{
	token new_start_token = start_token + "'";
	production new_production;
	new_production.left = new_start_token;
	new_production.right.push_back(start_token);
	productions.insert(productions.begin(),new_production);
	non_terminators.insert(non_terminators.begin(),new_start_token);
	start_token = new_start_token;
}

void grammar::ini_productions_class()
{
	int t = 0;//productions_class的初始化
	for (production i : productions)
	{
		productions_class[i.left].insert(t);
		t++;
	}
}

void grammar::infer_epsilon(const token& non_terminator) 
{
	for (int i : productions_class[non_terminator])//遍历该非终结符对应的所有产生式
	{
		bool is_epsilon = true;
		production current_production = productions[i];
		for (const auto& j : current_production.right)//判断该产生式右侧第一个字符
		{
			if (j == non_terminator)
				is_epsilon = false;
			else if (find(terminators.begin(), terminators.end(), j) == terminators.end())//若为非终结符
			{
				if (epsilon_over[j] == false)//递归计算j是否为空
				{
					infer_epsilon(j);
					epsilon_over[j] = true;
				}
				is_epsilon = is_epsilon || epsilon[j];//只有产生式均为不含自身非终结符且他们均可推空或直接推空，non_terminator才能推空
			}
			else if (j == "n")
				continue;
			else
				is_epsilon = false;
			if (is_epsilon == false)
				break;
		}
		if (is_epsilon == true)
		{
			epsilon[non_terminator] = true;
			return;
		}
	}
}

void grammar::output_epsilon()
{
	for (token i : non_terminators)
	{
		if (!epsilon_over[i])
			infer_epsilon(i);
		cout << i << ":" << epsilon[i] << endl;
	}
}

void grammar::first_derivation(const token& non_terminator)//计算first集
{
	for (int i : productions_class[non_terminator])//遍历该非终结符对应的所有产生式
	{
		production current_production = productions[i];
		bool all_epsilon = true;
		for (const auto& j : current_production.right)//遍历该产生式右侧字符
		{
			if (find(terminators.begin(), terminators.end(), j) != terminators.end())//若为终结符则加入该产生式first集，否则继续遍历
			{
				all_epsilon = false;
				first_of_production[i].insert(j);
				break;
			}
			if (j != non_terminator&&first_over[j] == false)//若为非终结符且为求取其first集，则递归求取
			{
				first_derivation(j);
			}
			first_of_production[i].insert(first[j].begin(), first[j].end());//若非终结符first集已计算则将其first集加入该产生式的first集
			if (!epsilon[j])//若不含空产生式则结束遍历
			{
				all_epsilon = false;
				break;
			}
			else//若含空产生式，删除first集中的ε，继续遍历
			{
				first_of_production[i].erase("ε");
			}
		}
		if (all_epsilon)//若都为空，则将其first加入ε
		{
			first_of_production[i].insert("ε");
		}
		first[non_terminator].insert(first_of_production[i].begin(), first_of_production[i].end());//将该产生式first集加入所求非终结符的first集
	}
	first_over[non_terminator] = true;
}

void grammar::follow_derivation(const token& non_terminator) 
{
	for (const auto& i : non_terminators)//遍历非终结符
	{
		for (int j : productions_class[i])//遍历该非终结符对应的产生式
		{
			const auto& current_production = productions[j];
			for (int t = 0,k;t < current_production.right.size();t++)//遍历产生式右侧所有字符
			{
				if (current_production.right[t] == non_terminator)//若遍历到该非终结符
				{
					for (k = t + 1;k < current_production.right.size();k++)//遍历该非终结符右侧所有字符
					{
						const auto& temp_token=current_production.right[k];
						if (find(terminators.begin(), terminators.end(), temp_token) != terminators.end())//若为终结符则加入follow集
						{
							follow[non_terminator].insert(temp_token);
							break;
						}
						follow[non_terminator].insert(first[temp_token].begin(), first[temp_token].end());//若为非终结符则将其first集加入该非终结符follow集中
						if (first[temp_token].find("ε") == first[temp_token].end())//如无空产生式，终止遍历
						{
							break;
						}
						else//否则将ε从follow集中删除，继续遍历
						{
							follow[non_terminator].erase("ε");
						}
					}
					if (k == current_production.right.size() && current_production.left != non_terminator)//如果该产生式遍历结束仍有空产生式
					{
						const auto& non_terminator2 = current_production.left;
						follow_include[non_terminator][non_terminator2] = true;
						if (!follow_include[non_terminator2][non_terminator])//若该产生式左侧不包含该非终结符
						{
							if (!(follow_over[non_terminator2]))
							{
								follow_derivation(non_terminator2);
							}
							follow[non_terminator].insert(follow[non_terminator2].begin(), follow[non_terminator2].end());//将该产生式左侧的follow集加入到该非终结符follow集中
						}
					}
				}
			}
		}
	}
	follow_over[non_terminator] = true;
}

void grammar::output_productions()//打印产生式
{
	for (production i : productions)
	{
		cout << i.left << "=";
		for(string j:i.right)
			cout<<j;
		cout << endl;
	}
	cout << endl;
}

void grammar::output_first()//计算所有first集
{
	for (const auto& i : productions)
	{
		first_of_production.push_back({});
	}
	for (token i : non_terminators)
	{
		if (!first_over[i])
			first_derivation(i);
	}
}

void grammar::output_follow()//计算所有follow集
{
	follow[start_token].insert("$");
	for (token i : non_terminators)
	{
		if(!follow_over[i])
			follow_derivation(i);
	}
	for (token i:non_terminators)
	{
		for (token j: non_terminators)
		{
			if (follow_include[i][j] && follow_include[j][i])
			{
				follow[i].insert(follow[j].begin(), follow[j].end());
				follow[j] = follow[i];
				follow_include[i][j] = follow_include[j][i] = false;
			}
		}
	}	
}

void grammar::output_first_follow()//打印first_follow集
{
	g.output_epsilon();
	g.output_first();
	g.output_follow();
	cout << "----------------------------------------------" << endl;
	cout << setiosflags(ios::left) << setw(15) << " " << setw(15) << "first" << setw(15) << "follow" << endl;
	cout << "----------------------------------------------" << endl;
	for (token i : non_terminators)
	{
		cout  << setw(15) << i + ": ";
		string fi = "{",fo="{";
		for (token j : first[i])
		{
			fi+=j+",";
		}
		fi=fi.substr(0, fi.size() - 1);
		fi += "}";
		for (token j : follow[i])
		{
			fo+=j+",";
		}
		fo=fo.substr(0, fo.size() - 1);
		fo += "}";
		cout << setw(15)<<fi<<setw(15)<<fo<<endl;
	}
}

grammar::grammar()//默认构造函数，读入文法
//:
//	non_terminators({ "E", "T", "F" }),
//	terminators(
//		{
//			"+", "-", "*", "/", "(", ")","n","ε","$"
//		}
//	),
//	start_token("E"),
//	productions( 
//		{
//			{
//				"E", {"E","+","T"}
//			},
//			{
//				"E", {"E","-","T"}
//			},
//			{
//
//				"E", {"T"}
//
//			},
//			{
//				"T", {"T","*","F"}
//			},
//			{
//				"T", {"T","/","F"}
//			},
//			{
//				"T", {"F"}
//			},
//			{
//				"F", {"(","E",")"}
//			},
//			{
//				"F", {"n"}
//			}
//		}
//	)
{
	FILE *fptr1;
	if (!(fptr1 = fopen("grammar.in", "r")))
	{
		printf("打开文件失败！");
		exit(0);
	}
	char buf[100];
	fgets(buf, 100, fptr1);
	token s=buf;
	s = s.substr(0, s.size() - 1);
	non_terminators = split(s);
	fgets(buf, 100, fptr1);
	s = buf;
	s = s.substr(0, s.size() - 1);
	terminators = split(s);
	terminators.push_back("ε");
	terminators.push_back("$");
	fgets(buf, 100, fptr1);
	start_token = buf[0];
	while (!feof(fptr1))
	{
		fgets(buf, 100, fptr1);
		token left = string(1,buf[0]);
		deque<token> right;
		for (int i = 3;buf[i] != '\n'&&buf[i]!=-'\0';i++)
		{
			right.push_back(string(1, buf[i]));
		}
		production p = {left,right};
		productions.push_back(p);
	}
	for (token i : non_terminators)
	{
		first_over.insert(pair<token, bool>(i, false));
		follow_over.insert(pair<token, bool>(i, false));
		epsilon_over.insert(pair<token, bool>(i, false));
	}
}