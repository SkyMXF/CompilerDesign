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
{//状态图边
public:
	string acceptSign;		//接收的符号
	class FAGraphNode *aimTo;		//目标结点
};

class FAGraphNode
{//状态图结点
public:
    int stateNum;
	vector<FAEdge> outEdgeList;	//出边列表
	bool terminal;				//是否是终态标识
	string signName;			//为终态时，匹配的sign类型名
};

class FAGraph
{//状态图
public:
	FAGraphNode *iniState;		//初始状态结点

	FAGraph()
	{
		//创建初始状态结点
		FAGraphNode *iniNode = new FAGraphNode;
		iniNode->stateNum = 0;
		iniNode->terminal = false;
		iniState = iniNode;
	}

	int addNode(FAGraphNode *src, string aSign, int stateNum, bool isTerm)
	{//以src为源结点，根据输入条件创建目标结点并建立连接
		return 0;
	}
	int linkNode(FAGraphNode *src, FAGraphNode *aim)
	{//以src为源结点，aim为目标结点建立连接
		return 0;
	}
	int breakLink(FAGraphNode *src, FAGraphNode *aim)
	{//切断src与aim结点的连接
		return 0;
	}
	int deleteNode(FAGraphNode *node)
	{//删除该结点及其所有出边，如果某个出边的目标结点仅有这一条入边，则对目标结点同样调用该函数
		return 0;
	}
};

class Mode
{
public:
	string name = "";
	vector<Sign> signList;
	FAGraph dfaGraph;			//DFA状态图
};

class SEULex
{
public:
    ifstream inFile;
    ofstream outhppFile;
    ofstream outcppFile;
    ofstream outmainFile;
    vector<Sign> signList;		//全局声明的标记
	vector<Mode> modeList;		//识别模式列表

	SEULex();

    int run();
    
    //读取lex文件部分
    int readGlobalDeclare();   //read first part of .lex
    int readMatchOperation();   //read second part of .lex
    int readCCode();            //read third part of .lex

    //从SignRule转换为DFA状态图部分
	int signruleToDFA();

	//代码生成
    //int stateGraphInit();       //build NFA graph
    //int NFAtoDFA();             //convert NFA to DFA
    int buildCode();            //build code by graph and sign rules
};