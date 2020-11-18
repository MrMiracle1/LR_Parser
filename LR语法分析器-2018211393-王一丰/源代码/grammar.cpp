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
	int t = 0;//productions_class�ĳ�ʼ��
	for (production i : productions)
	{
		productions_class[i.left].insert(t);
		t++;
	}
}

void grammar::infer_epsilon(const token& non_terminator) 
{
	for (int i : productions_class[non_terminator])//�����÷��ս����Ӧ�����в���ʽ
	{
		bool is_epsilon = true;
		production current_production = productions[i];
		for (const auto& j : current_production.right)//�жϸò���ʽ�Ҳ��һ���ַ�
		{
			if (j == non_terminator)
				is_epsilon = false;
			else if (find(terminators.begin(), terminators.end(), j) == terminators.end())//��Ϊ���ս��
			{
				if (epsilon_over[j] == false)//�ݹ����j�Ƿ�Ϊ��
				{
					infer_epsilon(j);
					epsilon_over[j] = true;
				}
				is_epsilon = is_epsilon || epsilon[j];//ֻ�в���ʽ��Ϊ����������ս�������Ǿ����ƿջ�ֱ���ƿգ�non_terminator�����ƿ�
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

void grammar::first_derivation(const token& non_terminator)//����first��
{
	for (int i : productions_class[non_terminator])//�����÷��ս����Ӧ�����в���ʽ
	{
		production current_production = productions[i];
		bool all_epsilon = true;
		for (const auto& j : current_production.right)//�����ò���ʽ�Ҳ��ַ�
		{
			if (find(terminators.begin(), terminators.end(), j) != terminators.end())//��Ϊ�ս�������ò���ʽfirst���������������
			{
				all_epsilon = false;
				first_of_production[i].insert(j);
				break;
			}
			if (j != non_terminator&&first_over[j] == false)//��Ϊ���ս����Ϊ��ȡ��first������ݹ���ȡ
			{
				first_derivation(j);
			}
			first_of_production[i].insert(first[j].begin(), first[j].end());//�����ս��first���Ѽ�������first������ò���ʽ��first��
			if (!epsilon[j])//�������ղ���ʽ���������
			{
				all_epsilon = false;
				break;
			}
			else//�����ղ���ʽ��ɾ��first���еĦţ���������
			{
				first_of_production[i].erase("��");
			}
		}
		if (all_epsilon)//����Ϊ�գ�����first�����
		{
			first_of_production[i].insert("��");
		}
		first[non_terminator].insert(first_of_production[i].begin(), first_of_production[i].end());//���ò���ʽfirst������������ս����first��
	}
	first_over[non_terminator] = true;
}

void grammar::follow_derivation(const token& non_terminator) 
{
	for (const auto& i : non_terminators)//�������ս��
	{
		for (int j : productions_class[i])//�����÷��ս����Ӧ�Ĳ���ʽ
		{
			const auto& current_production = productions[j];
			for (int t = 0,k;t < current_production.right.size();t++)//��������ʽ�Ҳ������ַ�
			{
				if (current_production.right[t] == non_terminator)//���������÷��ս��
				{
					for (k = t + 1;k < current_production.right.size();k++)//�����÷��ս���Ҳ������ַ�
					{
						const auto& temp_token=current_production.right[k];
						if (find(terminators.begin(), terminators.end(), temp_token) != terminators.end())//��Ϊ�ս�������follow��
						{
							follow[non_terminator].insert(temp_token);
							break;
						}
						follow[non_terminator].insert(first[temp_token].begin(), first[temp_token].end());//��Ϊ���ս������first������÷��ս��follow����
						if (first[temp_token].find("��") == first[temp_token].end())//���޿ղ���ʽ����ֹ����
						{
							break;
						}
						else//���򽫦Ŵ�follow����ɾ������������
						{
							follow[non_terminator].erase("��");
						}
					}
					if (k == current_production.right.size() && current_production.left != non_terminator)//����ò���ʽ�����������пղ���ʽ
					{
						const auto& non_terminator2 = current_production.left;
						follow_include[non_terminator][non_terminator2] = true;
						if (!follow_include[non_terminator2][non_terminator])//���ò���ʽ��಻�����÷��ս��
						{
							if (!(follow_over[non_terminator2]))
							{
								follow_derivation(non_terminator2);
							}
							follow[non_terminator].insert(follow[non_terminator2].begin(), follow[non_terminator2].end());//���ò���ʽ����follow�����뵽�÷��ս��follow����
						}
					}
				}
			}
		}
	}
	follow_over[non_terminator] = true;
}

void grammar::output_productions()//��ӡ����ʽ
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

void grammar::output_first()//��������first��
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

void grammar::output_follow()//��������follow��
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

void grammar::output_first_follow()//��ӡfirst_follow��
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

grammar::grammar()//Ĭ�Ϲ��캯���������ķ�
//:
//	non_terminators({ "E", "T", "F" }),
//	terminators(
//		{
//			"+", "-", "*", "/", "(", ")","n","��","$"
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
		printf("���ļ�ʧ�ܣ�");
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
	terminators.push_back("��");
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