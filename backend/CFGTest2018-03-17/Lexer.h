#pragma once
#include<string>
#include<fstream>
#include<iostream>
#include<sstream>
#include<map>
#include"globalVariable.h"
#include"FilePreProcess.h"
class Lexer
{
public:
	Lexer(std::string fileName);
	~Lexer();
	void InitKeyWordTable(); //��ʼ���ؼ��ֱ�
	void ChangeFunctionName();
	//�޸ĺ�����

	int Match();	//ƥ�䵱ǰtoken
	void BackToFileBegin();	//�˻��ļ�������ʼ�ĵط�
	std::string GetTokenStringValue();	//���ص�ǰtoken���ַ���ֱ����
	int GetToken() const;//���ص�ǰtoken
	double GetTokenValue() const;	//���ַ���������ʱ��������ֵ
	int GetIndex()const;	
	std::string GetString(int index); //����һ�ε��ַ����±��뵱ǰ�±��ȡ�ַ�������
	void BackToIndex(int index);//��scanner���ص�index�±괦
    int GetPrevTokenIndex() const; //������һ��token��index����
    long GetStringLength() const;   //����Դ�ַ����ܳ���

private:
	int m_iIndex;
	std::string m_sSrc;
	std::string m_sCurrentTokenStringValue;
	int m_iLengthOfSrc;	//Դ�ļ��ַ�������
	std::map<std::string, int>m_mapKeyWordTable;
	int m_iToken;	//���浱ǰtoken
	double m_dTokenValue;
	int m_iLastIndex;//���Խ�ȡ�ַ���
    int m_iPrevIndex;//��¼��һ��token��index
};