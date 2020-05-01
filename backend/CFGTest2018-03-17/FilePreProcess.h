#ifndef AUTO_SOFTWARE_TEST_FILE_PREPROCESS_H_
#define AUTO_SOFTWARE_TEST_FILE_PREPROCESS_H_


/*
*这个文件预处理类的主要功能如下：
*1、将if-else标准化，即没有花括号，补齐花括号，没有if else匹配，补齐if-else
*2、if-elseif-else转化成if-else结构
*3、switch结构把原来case之后没有break的语句全部转化为有break的，而不改变其逻辑结构
*/
#include<iostream>
#include<stack>
#include<string>
#include<map>
#include<vector>
#include"globalVariable.h"

using std::cin;
using std::cout;
using std::endl;
using std::stack;
using std::string;

class FilePreProcess
{
public:
	//FilePreProcess();
	FilePreProcess(string& localSrc);
	~FilePreProcess();
	string& GetSrc();
	int GetToken();
	int GetIndex();
	void InsertCode(int targetIndex,std::string code);
    void InsertChar();
	void IfElsePair();
	//检查if-else是否匹配，且都有{}
	void IfElsePairHelp(bool isIfCall);
	void IfElsePairProgram();
	void NestProcess(bool isIfCall);
	//void getFirstFunction();
	//获取第一个函数，目前只支持一个函数，所以在用户随便输入的情况下，我们只截取第一个函数
	//修改函数名
	void SwitchProgram();//对switch进行预处理
	void SwitchInBlock();
	void WhileAndForProgram();//对while和for语句进行预处理

	void BackToFileBegin();
	int Match();//match 一个token
	void PutBack();//往回退一个token
	void Program();
private:
	string& m_sSrc;
	int m_iIndex;
	int m_iOldIndex;
	int m_iMaxSize;
	int m_iToken;
	int m_iOldToken;
	// int m_iCounter;
	std::map<std::string, int> m_structKeyWordTable;
	double m_dTokenValue;
};

#endif // !AUTO_SOFTWARE_TEST_FILE_PREPROCESS_H_