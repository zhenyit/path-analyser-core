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
	void InitKeyWordTable(); //初始化关键字表
	void ChangeFunctionName();
	//修改函数名

	int Match();	//匹配当前token
	void BackToFileBegin();	//退回文件的最起始的地方
	std::string GetTokenStringValue();	//返回当前token的字符串直接量
	int GetToken() const;//返回当前token
	double GetTokenValue() const;	//当字符串是数字时，返回数值
	int GetIndex()const;	
	std::string GetString(int index); //将上一次的字符串下标与当前下标截取字符串返回
	void BackToIndex(int index);//让scanner返回到index下标处
    int GetPrevTokenIndex() const; //返回上一个token的index坐标
    long GetStringLength() const;   //返回源字符串总长度

private:
	int m_iIndex;
	std::string m_sSrc;
	std::string m_sCurrentTokenStringValue;
	int m_iLengthOfSrc;	//源文件字符串长度
	std::map<std::string, int>m_mapKeyWordTable;
	int m_iToken;	//储存当前token
	double m_dTokenValue;
	int m_iLastIndex;//用以截取字符串
    int m_iPrevIndex;//记录上一个token的index
};