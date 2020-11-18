#pragma once
#include<set>
#include<vector>
#include<map>
#include<deque>
#include<string>

using namespace std;

typedef string token;//��������

struct production {		//����ʽ���ݽṹ
	token left;	//�����ս��
	deque<token> right;	//�Ҳ����ʽ
};

class grammar {		//�ķ���
public:
	vector<token> non_terminators, terminators;		//���ս�������ս����
	token start_token;		//�ķ���ʼ��
	vector<production> productions;		//�ķ����в���ʽ
	map<token, bool> first_over, follow_over,epsilon_over;		//��¼���ս��first��follow���Ƿ�����
	map<token, set<token>> first,follow;		//���ս����first��follow��
	map<token, bool> epsilon;
	map<token, set<int>> productions_class;			//������ʽ�����ս��Ϊ����ʽ����
	vector<set<token>> first_of_production;		//����ʽ��first��
	map<token, map<token, bool>> follow_include;		//follow���Ļ��������ϵ
public:
	grammar();		//Ĭ�Ϲ��캯����ͨ���ļ������ķ�
	void infer_epsilon(const token&);//�Ƶ����ƿշ���
	void output_epsilon();//������ƿշ��ս��
	void first_derivation(const token&);//��Ӧ���ս��first�Ƶ�
	void follow_derivation(const token&);//��Ӧ���ս��follow���Ƶ�
	void output_productions();//������в���ʽ
	void output_first();//����first��
	void output_follow();//����follow��
	void output_first_follow();//��ӡfirst��follow��
	void extend(); //�ع��ķ�
	void ini_productions_class();//��ʼ��productions_class
};

extern grammar g;//ȫ���ķ�
