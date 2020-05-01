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
    void InsertCode();//将所有代码整合输出


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
    const std::string k_m_sPath = "std::string path = \"\";";		//路径通过string量来维护
    const std::string k_m_sFitness = "double globalFitness = 0;";
    const std::string k_m_sFunctionDefine = "void DEAlgorithm();";
    std::string m_sDEAlgorithmString;

    int m_iNumOfParameter;
    //函数参数数量

    int m_iNumOfPath;
    //路径数量
    std::string m_sTargetFile = "processedFile.cpp";

    std::stringstream m_ssTestFunctionInsertCode;
    //用户输入的测试函数经过处理后变成的函数字符串
    std::stringstream m_ssFitnessCalculateFunctionCode;
    //将判断语句经过转换后对应的fitness计算函数

    Lexer m_classLexer;
    Graph m_classCFG;
    CompilerCall compiler_call;        //首字母
    std::vector<double> m_vectorArgumentBound;          //实参上下界数组

    //std::vector<int> nodeStatus;//存语法结构


    std::unordered_map<std::string, int> HashTableOfData;
    //存储函数中声明的变量类型和变量名

    int m_iConditionalNode;
    //判断节点个数
    const std::string NodeTable();

    void BuildCFG(int rootVertex,bool isElseCall = false,int condition_node=0, bool is_while_call = false, int while_condition_node = -1);
    //匹配ENUM_KEYWORD::IF ENUM_KEYWORD::WHILE ENUM_KEYWORD::FOR ENUM_KEYWORD::SWITCH语句
    int BuildCFGOfSwitch(int rootVertex, string switch_condition = NULL, bool is_while_call = false, int while_condition_node = -1);
    //匹配switch里面的case和default，并返回终止节点


    //输出文件代码
    //将代码进行输出
    //void Output

    




    bool IsBreakVertex(const std::vector<int>& breakVertexes, int vertex);
    void CalculatePathNum();

    bool IsDataType(int tokenType);
    //判断当前token是否为基本数据类型关键字
    //生成DEAlgorithm的一个string用于代码插入

    //获取算法源代码
    void DEAlgorithm(int nodeNum, int pathNum, int R, double* lb, double* ub);
    std::string IntToString(int x);
    std::string DoubleToString(double x);
    //fitness计算函数发射函数
    nsFitness::CF m_fEmitCalculateFitnessFunction;
    //fitness计算函数发射对象声明结束
    std::vector<double> Split(char* data);

};
#endif // !AUTO_SOFTWARE_TEST_CODE_INSERT_H_