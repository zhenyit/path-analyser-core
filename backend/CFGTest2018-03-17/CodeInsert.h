//#pragma once

#ifndef AUTO_SOFTWARE_TEST_CODE_INSERT_H_

#define AUTO_SOFTWARE_TEST_CODE_INSERT_H_

#include<string>
#include<fstream>
#include<sstream>
#include<vector>
#include<stack>
#include<iostream>
#include<fstream>
#include<unordered_map>
#include<stack>
#include"Lexer.h"
#include"globalVariable.h"
#include"fitness.h"
#include"Graph.h"
#include"compilerCall.h"	

class CodeInsert
{
public:
    //CodeInsert();
    CodeInsert(std::string sourceFileName, char* argv[]);
    ~CodeInsert();
    void Program();
    std::string Insert();
    //std::vector<int> getNodeStatus();
    void InsertCode();//�����д����������


private:
  
    const std::string k_m_sHfile = "#include<iostream>\r\n"
        "#include<fstream>\r\n"
        "#include<ctime>\r\n"
        "#include<cstdlib>\r\n"
        "#include<vector>\r\n"
        "#include<string>\r\n"
        "#include<cmath>\r\n"
        "#include<algorithm>\r\n"
        "using std::string;\r\nusing std::cout;\r\nusing std::cin;\r\nusing std::endl;\r\nusing std::vector;\r\nusing std::fstream;\r\n";
    const std::string k_m_sPathTable = "std::vector<string>pathTable;";
    const std::string k_m_sPath = "std::string path = \"\";";		//·��ͨ��string����ά��
    const std::string k_m_sFitness = "double globalFitness = 0;";
    const std::string k_m_sFunctionDefine = "void DEAlgorithm();";
    std::string m_sDEAlgorithmString;

    int m_iNumOfParameter;
    //������������

    int m_iNumOfPath;
    //·������
    std::string m_sTargetFile = "processedFile.cpp";

    std::stringstream m_ssTestFunctionInsertCode;
    //�û�����Ĳ��Ժ�������������ɵĺ����ַ���
    std::stringstream m_ssFitnessCalculateFunctionCode;
    //���ж���侭��ת�����Ӧ��fitness���㺯��

    Lexer m_classLexer;
    Graph m_classCFG;
    CompilerCall compiler_call;        //����ĸ
    std::vector<double> m_vectorArgumentBound;          //ʵ�����½�����

    //std::vector<int> nodeStatus;//���﷨�ṹ


    std::unordered_map<std::string, int> HashTableOfData;
    //�洢�����������ı������ͺͱ�����

    int m_iConditionalNode;
    //�жϽڵ����
    const std::string NodeTable();

    void BuildCFG(int rootVertex,bool isElseCall = false,int condition_node=0, bool is_while_call = false, int while_condition_node = -1);
    //ƥ��ENUM_KEYWORD::IF ENUM_KEYWORD::WHILE ENUM_KEYWORD::FOR ENUM_KEYWORD::SWITCH���
    int BuildCFGOfSwitch(int rootVertex, string switch_condition = NULL, bool is_while_call = false, int while_condition_node = -1);
    //ƥ��switch�����case��default����������ֹ�ڵ�


    //����ļ�����
    //������������
    //void Output

    




    bool IsBreakVertex(const std::vector<int>& breakVertexes, int vertex);
    void CalculatePathNum();

    bool IsDataType(int tokenType);
    //�жϵ�ǰtoken�Ƿ�Ϊ�����������͹ؼ���
    //����DEAlgorithm��һ��string���ڴ������

    //��ȡ�㷨Դ����
    void DEAlgorithm(int nodeNum, int pathNum, int R, double* lb, double* ub);
    std::string IntToString(int x);
    std::string DoubleToString(double x);
    //fitness���㺯�����亯��
    nsFitness::CF m_fEmitCalculateFitnessFunction;
    //fitness���㺯�����������������
    std::vector<double> Split(char* data);

};
#endif // !AUTO_SOFTWARE_TEST_CODE_INSERT_H_