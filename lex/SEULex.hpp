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

class Mode
{
public:
	string name = "";
	vector<Sign> signList;
};

class GraphNode
{//State graph node
public:
    int stateNum;
    GraphNode *firstAim;  //first aim state
    GraphNode *next;      //next state which has the same source state
};

class SEULex
{
public:
    ifstream inFile;
    ofstream outhppFile;
    ofstream outcppFile;
    ofstream outmainFile;
    vector<Sign> signList;		//全局声明的标记
	vector<Mode> modeList;		//状态列表
    GraphNode *iniState;        //first node of state graph

	SEULex();

    int run();
    
    //read file part
    int readGlobalDeclare();   //read first part of .lex
    int readMatchOperation();   //read second part of .lex
    int readCCode();            //read third part of .lex

    //graph part
    int stateGraphInit();       //build NFA graph
    int NFAtoDFA();             //convert NFA to DFA
    int buildCode();            //build code by graph and sign rules
};