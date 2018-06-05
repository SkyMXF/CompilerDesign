#pragma once
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
using namespace std;

class Sign
{//sign defined in .lex
public:
    string name = "";        //name of this sign
    string rule = "";        //rule of this sign
    string operation = "";   //operation to do after match this sign
};

class FAEdge
{//״̬ͼ��
public:
	string acceptSign;		//���յķ���
	class FAGraphNode *aimTo;		//Ŀ����
};

class FAGraphNode
{//״̬ͼ���
public:
    int stateNum;
	vector<FAEdge> outEdgeList;	//�����б�
	bool terminal;				//�Ƿ�����̬��ʶ
	string signName;			//Ϊ��̬ʱ��ƥ���sign������
};

class FAGraph
{//״̬ͼ
public:
	FAGraphNode *iniState;		//��ʼ״̬���

	FAGraph()
	{
		//������ʼ״̬���
		FAGraphNode *iniNode = new FAGraphNode;
		iniNode->stateNum = 0;
		iniNode->terminal = false;
		iniState = iniNode;
	}

	int addNode(FAGraphNode *src, string aSign, int stateNum, bool isTerm)
	{//��srcΪԴ��㣬����������������Ŀ���㲢��������
		return 0;
	}
	int linkNode(FAGraphNode *src, FAGraphNode *aim)
	{//��srcΪԴ��㣬aimΪĿ���㽨������
		return 0;
	}
	int breakLink(FAGraphNode *src, FAGraphNode *aim)
	{//�ж�src��aim��������
		return 0;
	}
	int deleteNode(FAGraphNode *node)
	{//ɾ���ý�㼰�����г��ߣ����ĳ�����ߵ�Ŀ���������һ����ߣ����Ŀ����ͬ�����øú���
		return 0;
	}
};

class Mode
{
public:
	string name = "";
	vector<Sign> signList;
	FAGraph dfaGraph;			//DFA״̬ͼ
};

class SEULex
{
public:
    ifstream inFile;
    ofstream outhppFile;
    ofstream outcppFile;
    ofstream outmainFile;
    vector<Sign> signList;		//ȫ�������ı��
	vector<Mode> modeList;		//ʶ��ģʽ�б�

	SEULex();

    int run();
    
    //��ȡlex�ļ�����
    int readGlobalDeclare();   //read first part of .lex
    int readMatchOperation();   //read second part of .lex
    int readCCode();            //read third part of .lex

    //��SignRuleת��ΪDFA״̬ͼ����
	int signruleToDFA();

	//��������
    //int stateGraphInit();       //build NFA graph
    //int NFAtoDFA();             //convert NFA to DFA
    int buildCode();            //build code by graph and sign rules
};