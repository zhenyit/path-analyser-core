#ifndef AUTO_SOFTWARE_TEST_FILE_PREPROCESS_H_
#define AUTO_SOFTWARE_TEST_FILE_PREPROCESS_H_


/*
*����ļ�Ԥ���������Ҫ�������£�
*1����if-else��׼������û�л����ţ����뻨���ţ�û��if elseƥ�䣬����if-else
*2��if-elseif-elseת����if-else�ṹ
*3��switch�ṹ��ԭ��case֮��û��break�����ȫ��ת��Ϊ��break�ģ������ı����߼��ṹ
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
	//���if-else�Ƿ�ƥ�䣬�Ҷ���{}
	void IfElsePairHelp(bool isIfCall);
	void IfElsePairProgram();
	void NestProcess(bool isIfCall);
	//void getFirstFunction();
	//��ȡ��һ��������Ŀǰֻ֧��һ���������������û�������������£�����ֻ��ȡ��һ������
	//�޸ĺ�����
	void SwitchProgram();//��switch����Ԥ����
	void SwitchInBlock();
	void WhileAndForProgram();//��while��for������Ԥ����

	void BackToFileBegin();
	int Match();//match һ��token
	void PutBack();//������һ��token
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