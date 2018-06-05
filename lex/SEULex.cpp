#include "SEULex.hpp"

SEULex::SEULex()
{
	Mode iniMode;
	iniMode.name = "INITIAL";
	modeList.push_back(iniMode);
}

int SEULex::run()
{
	int rc;
    //read file part
	outmainFile << "/*********user define variable and funcion**********/\n";
    rc = readGlobalDeclare();   //read first part of .lex
	if (rc) return rc;
    rc = readMatchOperation();   //read second part of .lex
	if (rc) return rc;
    rc = readCCode();            //read third part of .lex
	if (rc) return rc;

    //graph part
    stateGraphInit();       //build NFA graph
    NFAtoDFA();             //convert NFA to DFA
    buildCode();            //build code by graph and sign rules
    
    return 0;
}

bool is_space(const char &c)
{
	if (c == ' ') return true;
	if (c == '\t') return true;
	if (c == '\0') return true;
	return false;
}
bool is_letter(const char &c)
{
	if (c >= 'a' && c <= 'z') return true;
	if (c >= 'A' && c <= 'Z') return true;
	if (c == '_') return true;
	return false;
}
int SEULex::readGlobalDeclare()
{//read first part of .lex

	//read lex part1 fa
	class FA {
	public:
		int state = 0;
		int receive(const char &c)
		{
			switch (state)
			{
			case 0:
				if (is_space(c)) return state;
				if (c == '\n') return state;
				if (c == '%') return state = 1;
				return state = -1;
			case 1:
				if (c == '{') return state = 2;
				if (c == '%') return state = 11;
				if (c == 'x') return state = 9;
				return state = -1;
			case 2:
				if (c == '%') return state = 3;
				return state;
			case 3:
				if (c == '}') return state = 4;
				return state = 2;
			case 4:
				if (is_space(c) || c == '\n') return state;
				if (is_letter(c)) return state = 5;
				if (c == '%') return state = 8;
				return state = -1;
			case 5:
				if (is_letter(c)) return state;
				if (is_space(c)) return state = 6;
				return state = -1;
			case 6:
				if (is_space(c)) return state;
				if (c == '\n') return state = -1;
				return state = 7;
			case 7:
				if (c == '\n') return state = 4;
				return state;
			case 8:
				if (c == 'x') return state = 9;
				if (c == '%') return state = 11;
				return state = -1;
			case 9:
				if (is_space(c)) return state;
				if (is_letter(c)) return state = 10;
				return state = -1;
			case 10:
				if (is_letter(c)) return state;
				if (c == '\n') return state = 4;
				return state = -1;
			case 11:
				return state = -1;
			}
			return state = -1;
		}
	};

	char nowChar;
	inFile >> noskipws;			//设定ifstream读取不忽略空格和\n
	
	FA fa;
	Mode nowMode;		//暂存读取的mode信息
	Sign nowSign;		//暂存读取的sign的信息
	int nowState = 0, preState = 0;		//保存FA状态
	while (true)		//开始读取
	{
		//从输入流读入下一个字符
		inFile >> nowChar;
		if (inFile.eof())	//输入的lex编写格式错误
		{
			cout << "read .lex file error in part 1\n";
			return -1;
		}

		//FA读入一个字符
		nowState = fa.receive(nowChar);
		
		if (nowState == 2 && preState == 2)			//正在读取C全局声明
			outmainFile << nowChar;
		else if (nowState == 2 && preState == 3)	//正在读取C全局声明
			outmainFile << '%' << nowChar;
		else if (nowState == 5 && preState == 4)	//开始读取一行lex的Sign声明
		{
			nowSign.name = nowChar;
			nowSign.rule = "";
		}
		else if (nowState == 5)				//正在读取一行lex声明的Sign.name
			nowSign.name += nowChar;
		else if (nowState == 7)				//正在读取一行lex声明的Sign.rule
			nowSign.rule += nowChar;
		else if (nowState == 4 && preState == 7)	//一行lex的Sign声明读取完毕
			signList.push_back(nowSign);
		else if (nowState == 10 && preState == 9)	//开始读取一行lex的Mode声明
			nowMode.name = nowChar;
		else if (nowState == 10)			//正在读取一行lex的Mode声明
			nowMode.name += nowChar;
		else if (nowState == 4 && preState == 10)	//一行lex的Mode声明读取完毕
			modeList.push_back(nowMode);
		else if (nowState == 11)
			return 0;
		else if (nowState == -1)	//输入的lex编写格式错误
		{
			cout << "read .lex file error in part 1\n";
			return -1;
		}

		preState = nowState;
	}

   return 0;
}

int SEULex::readMatchOperation()
{//read second part of .lex
    
	class FA {
	public:
		int state = 0;
		int counter = 0;
		/******************************************************
		 * 考虑到用户所需要执行的代码内可能包括'{'
		 * 而此处用语法分析的方法来解决过于复杂
		 * 因此仅使用该变量检查是否为最后一个'}'字符时使用
		 * 同时规定用户的输入必须满足'{'与'}'一一对应
		*******************************************************/
		int receive(const char &c)
		{
			switch (state)
			{
			case 0:
				if (is_space(c) || c == '\n') return state;
				if (c == '{') return state = 1;
				if (c == '"') return state = 3;
				if (c == '\\') return state = 6;
				if (c == '.') return state = 8;
				if (c == '<') return state = 11;
				if (c == '%') return state = 13;
				return state = -1;
			case 1:
				if (is_letter(c)) return state;
				if (c == '}') return state = 2;
				return state = -1;
			case 2:
				if (is_space(c)) return state;
				if (c == '{') return state = 9;
				return state = -1;
			case 3:
				if (c == '\\') return state = 4;
				if (c == '"') return state = 5;
				return state;
			case 4:
				return state = 3;
			case 5:
				if (is_space(c)) return state;
				if (c == '{') return state = 9;
				return state = -1;
			case 6:
				if (c == 'n') return state = 7;
				return state = -1;
			case 7:
				if (is_space(c)) return state;
				if (c == '{') return state = 9;
				return state = -1;
			case 8:
				if (is_space(c)) return state;
				if (c == '{') return state = 9;
				return state = -1;
			case 9:
				if (c == '}') return state = 10;
				return state;
			case 10:
				if (is_space(c)) return state;
				if (c == '\n') return state = 0;
				if (c == '%') return state = 13;
				return state = -1;
			case 11:
				if (is_letter(c)) return state;
				if (c == '>') return state = 12;
				return state = -1;
			case 12:
				if (c == '{') return state = 1;
				if (c == '"') return state = 3;
				if (c == '\\') return state = 6;
				if (c == '.') return state = 8;
				return state = -1;
			case 13:
				if (c == '%') return state = 14;
				return state = -1;
			case 14:
				return state = -1;
			}

			return state = -1;
		}
	};

	char nowChar = 'a';
	inFile >> noskipws;			//设定ifstream读取不忽略空格和\n

	do inFile >> nowChar;
	while (is_space(nowChar));	//找到第一个有效字符

	FA fa;
	string mName = "";			//暂存读取的mode.name的信息
	Sign nowSign;				//暂存读取的sign
	Mode *pMode = &modeList[0];			//指向需要修改的mode
	int nowState = 0, preState = 0;		//保存FA状态
	while (true)		//开始读取
	{
		//从输入流读入下一个字符
		inFile >> nowChar;
		if (inFile.eof())	//输入的lex编写格式错误
		{
			cout << "read .lex file error in part 2\n";
			return -1;
		}
		//FA读入一个字符
		nowState = fa.receive(nowChar);

		if (nowState == 1 && preState == 0)			//开始读取一个sign.name
			nowSign.name = "";
		else if (nowState == 1)				//正在读取一个sign.name
			nowSign.name += nowChar;
		else if (nowState == 2 && preState == 1)	//一个sign.name读取完毕
		{
			//在signList找到对应名称的sign，获取其rule
			for (int i = 0; i < signList.size(); i++)
			{
				if (nowSign.name == signList[i].name)
				{
					nowSign.rule = signList[i].rule;
					break;
				}
			}
			if (nowSign.rule == "")		//未找到sign
			{
				cout << "Cannot find a sign.\n";
				return -1;
			}
		}
		else if (nowState == 3 && preState == 0)	//开始读取一个未声明的sign.name
			nowSign.name = "%";			//%开头的sign代表未进行过全局声明的，纯字符表示的sign
		else if (nowState == 3)			//正在读取一个未声明的sign.name
			nowSign.name += nowChar;
		else if (nowState == 6)			//该sign为\n
			nowSign.name = ".endl";		//.开头的sign表示换行符\n或.
		else if (nowState == 8 && (preState == 0 || preState == 12))	//该sign为.
			nowSign.name = "..";
		else if (nowState == 9 && preState != 9)	//开始读取一个sign.operation
			nowSign.operation = "";
		else if (nowState == 9)			//正在读取一个sign.operation
			nowSign.operation += nowChar;
		else if (nowState == 10 && preState == 9)	//一个sign.name及operation读取完毕
		{
			pMode->signList.push_back(nowSign);
			nowSign.name = "";
			nowSign.operation = "";
			nowSign.rule = "";
			pMode = &modeList[0];		//pMode重置为默认模式
		}
		else if (nowState == 11 && preState == 0)	//开始读取一个特定mode下的sign
			mName = "";
		else if (nowState == 11)		//正在读取一个特定mode下的sign
			mName += nowChar;
		else if (nowState == 12)		//mode名称读取完毕，找到对应的mode，重定向pMode
		{
			int i;
			for (i = 0; i < modeList.size(); i++)
			{
				if (mName == modeList[i].name)
				{
					pMode = &modeList[i];
					break;
				}
			}
			if (i == modeList.size())
			{
				cout << "Cannot find a mode.\n";
				return -1;
			}
		}
		else if (nowState == 14)
			return 0;
		else if (nowState == -1)
		{
			cout << "read .lex file error in part 2\n";
			return -1;
		}
		preState = nowState;
	}
    
    return 0;
}

int SEULex::readCCode()
{//read third part of .lex

    /***复制inFile中的剩余内容，输出至outmainFile流中***/

	outmainFile << "/*********user define main function**********/\n";

	inFile >> noskipws;				//don't ignore whitescpace and newline'\n'
	char c;
	do {
		inFile >> c;
		if (!inFile.eof()) outmainFile << c;
		else break;
	} while (true);
    
    return 0;
}

int SEULex::stateGraphInit()
{//build NFA graph

    return 0;
}

int SEULex::NFAtoDFA()
{//convert NFA to DFA on graph

    return 0;
}

int SEULex::buildCode()
{//build code by graph and sign rules

    return 0;
}
