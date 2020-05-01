#include "stdafx.h"
#include "CodeInsert.h"


//驱动整个函数的执行
void CodeInsert::Program()
{
    //编译processfile.cpp并运行processfile.exe
    Insert();
    compiler_call.call();
}


CodeInsert::CodeInsert(std::string sourceFileName, char* argv[])
    : m_classLexer(sourceFileName), compiler_call(m_sTargetFile)
{
    m_iNumOfParameter = 0;
    m_iNumOfPath = 1;
    m_iConditionalNode = 0;
    //处理
    m_vectorArgumentBound.push_back(0);

}

CodeInsert::~CodeInsert() {}


//获取算法源代码，即通过这个函数将算法源代码输出
void CodeInsert::DEAlgorithm(int node_num, int pathNum, int R, double* lb, double* ub)
{
    using std::string;
    std::string s = "";


    std::string s1 =
        "void DEAlgorithm()"
        "{"
        "srand((unsigned)time(NULL));"
        "	fstream output(\"result.txt\");"
        "int lb[R];		"
        "int ub[R];		";
    //测试用例范围初始化

    cout << "上下界：" << endl;
    for (int i = 0; i < R * 2; i+=2)
    {
        s1.append("lb[");
        s1.append(IntToString(i/2));
        s1.append("]=");
        s1.append(DoubleToString(m_vectorArgumentBound[i]));
        s1.append(";");
        s1.append("ub[");
        s1.append(IntToString(i/2));
        s1.append("]=");
        s1.append(DoubleToString(m_vectorArgumentBound[i+1]));
        s1.append(";");
        cout << DoubleToString(m_vectorArgumentBound[i]) 
            << " " << DoubleToString(m_vectorArgumentBound[i + 1]) << endl;
    }


    s1 = s1 + "time_t start;"
        "time_t finish;"
        "for (int run = 0; run < RUN; run++)"
        "{"
        "pathTable.clear();"
        "for (int i = 0; i < NODENUM; i++)"
        "{"
        "for (int j = 0; j < 2; j++)"
        "{"
        "nodeTable[i][j] = false;"
        "}"
        "}"
        "output << endl;"
        "cout << \"Times: \" << run << endl;"
        "output << \"Times: \" << run << \" start:\" << endl;"
        "int x[POPNUM][R] = { 0 };	"
        "int v[POPNUM][R] = { 0 };	"
        "double fitness_x[POPNUM];	"
        "double fitness_v[POPNUM];	"
        "for (int i = 0; i < POPNUM; i++)"
        "{"
        "fitness_v[i] = fitness_x[i] = -1;"
        "}"
        "double solution[PATHNUM][R];"
        "int currentPathNum = 0;"
        "start = clock();	"

        "for (int i = 0; i < POPNUM; i++)"
        "{"
        "for (int j = 0; j < R; j++)"
        "{"
        "x[i][j] = (rand() % (ub[j] - lb[j] + 1)) + lb[j];"
        "}"
        "caseNum[run]++;";
    //这里需要插入测试函数调用代码
    string functionCallFirst = "test_function(";
    for (int j = 0; j < R; j++)
    {
        functionCallFirst.append("x[i][");
        functionCallFirst.append(IntToString(j));
        if (j != R - 1)
            functionCallFirst.append("],");
        else
            functionCallFirst.append("]);");
    }
    s1 += functionCallFirst;
    //函数调用结束，nodeTable与pathTable已经更新

    //计算当前属于第几条path
    string pathCalculate = "globalFitness = 0; vector<string>::iterator it;"
        "bool findIt = false;"
        "for (it = pathTable.begin(); it < pathTable.end(); it++)"
        "{"
        "if (*it == path)"
        "{ "
        "findIt = true;"
        "path = \"\";"
        "break;"
        "}"
        "}"
        "if (!findIt)"
        "{"
        "pathTable.push_back(path);"
        "path = \"\";"
        "for (int j = 0; j < R; j++)"
        "{"
        "solution[currentPathNum][j] = x[i][j];"
        "}"

        "currentPathNum++;"
        "}"
        "if (currentPathNum == PATHNUM)"
        "{"
        "break;"
        "}}";

    s = s + s1 + pathCalculate;//变量常量初始化，初始化种群代码
    s += "cycle[run] = 1;";
    //种群进行进化代码
    string sEvolution = "while (caseNum[run] <= MCN && currentPathNum < PATHNUM)"
        "{"
        "for (int i = 0; i < POPNUM; i++)"
        "{"
        "int k1 = rand() % 50;"
        "while (k1 == i)"
        "{"
        "k1 = rand() % 50;"
        "}"
        "int k2 = rand() % 50;"
        "while (k2 == i || k2 == k1)"
        "{"
        "k2 = rand() % 50;"
        "}"
        "int jrand = rand() % R;"
        "for (int j = 0; j < R; j++)"
        "{"
        "v[i][j] = (int)round(abs(x[i][j] + F * (x[k1][j] - x[k2][j])));"
        "if (rand() / (double)RAND_MAX <= PC && j != jrand)"
        "{"
        "v[i][j] = x[i][j];"
        "}"
        "if (v[i][j] > ub[j] || v[i][j] < lb[j])"
        "{"
        "v[i][j] = lb[j] + rand() % (ub[j] - lb[j] + 1);"
        "}"
        "}"
        "caseNum[run] = caseNum[run] + 1;";
    //交叉变异借宿，进行函数调用
    string functionCallSecond = "test_function(";
    for (int j = 0; j < R; j++)
    {
        functionCallSecond.append("v[i][");
        functionCallSecond.append(IntToString(j));
        if (j != R - 1)
            functionCallSecond.append("],");
        else
            functionCallSecond.append("]);");
    }
    //函数调用代码结束
    functionCallSecond += "globalFitness = 0;";
    //路径计算代码
    sEvolution = sEvolution + functionCallSecond;
    string pathCalculateSecond = "globalFitness = 0; vector<string>::iterator it;"
        "bool findIt = false;"
        "for (it = pathTable.begin(); it < pathTable.end(); it++)"
        "{"
        "if (*it == path)"
        "{ "
        "findIt = true;"
        "path = \"\";"
        "break;"
        "}"
        "}"
        "if (!findIt)"
        "{"
        "pathTable.push_back(path);"
        "path = \"\";"
        "for (int j = 0; j < R; j++)"
        "{"
        "solution[currentPathNum][j] = v[i][j];"
        "}"

        "currentPathNum++;"
        "}"
        "if (currentPathNum == PATHNUM)"
        "{"
        "break;"
        "}}";
    sEvolution += pathCalculateSecond;
    //Fitness 计算代码
    string fitnessCalculate =
        "for (int i = 0; i < POPNUM; i++)"
        "{"
        "if (currentPathNum == PATHNUM)"
        "{"
        "cycle[run]++;"
        "break;"
        "}";
    string functionCallFirstTrue = "test_function(";
    for (int j = 0; j < R; j++)
    {
        functionCallFirstTrue.append("x[i][");
        functionCallFirstTrue.append(IntToString(j));
        if (j != R - 1)
            functionCallFirstTrue.append("],");
        else
            functionCallFirstTrue.append("]);");
    }

    functionCallFirstTrue += "fitness_x[i] = globalFitness;"
        "path = \"\";"
        "globalFitness = 0;";

    string functionCallSecondTrue = "test_function(";
    for (int j = 0; j < R; j++)
    {
        functionCallSecondTrue.append("v[i][");
        functionCallSecondTrue.append(IntToString(j));
        if (j != R - 1)
            functionCallSecondTrue.append("],");
        else
            functionCallSecondTrue.append("]);");
    }
    functionCallSecondTrue += "fitness_v[i] = globalFitness;"
        "path = \"\";"
        "globalFitness = 0;";
    string fitnessCompare = "if (fitness_v[i] > fitness_x[i])"
        "{"
        "for (int j = 0; j < R; j++)"
        "x[i][j] = v[i][j];"
        "fitness_x[i] = fitness_v[i];"
        "}"
        "}";
    string end = "cycle[run]++;"
        "}";
    //交叉编译源代码
    sEvolution = sEvolution + fitnessCalculate + functionCallFirstTrue + functionCallSecondTrue + fitnessCompare + end;
    //测试报告输出源代码
    string testResult = "finish = clock();"
        "runTime[run] = double(finish - start) * 1000 / CLOCKS_PER_SEC;"
        "output << \"Run Time = \" << runTime[run] << \"ms\" << endl;"
        "output << \"NO. of cycles= \" << (cycle[run] - 1) << endl;"
        "coverage[run] = currentPathNum * 100 / PATHNUM;"
        "output << \"Path Coverage Percentage= \" << coverage[run] << \"%\" << endl;"
        "output << \"Best Solution\" << endl;"
        "for (int k = 0; k < PATHNUM; k++)"
        "{"
        "if (k < currentPathNum)"
        "{"
        "output << \"path \" << pathTable[k] <<\": \";"
        "for (int j = 0; j < R; j++)"
        "output << solution[k][j] << \" \";"
        "output << endl;"
        "}"
        "else\r\n"
        "{output << \"path\" << k << \": no cover.\" << endl;}"
        "}"
        "output << endl << endl;"
        "}"
        "}";
    m_sDEAlgorithmString = s + sEvolution + testResult;
}

std::string CodeInsert::IntToString(int x)
{
 /*   char buffer[20];
    _itoa_s(x, buffer, 10);
    std::string buf(buffer);
    return buf;*/

    std::stringstream ss;
    string str;
    ss << x;
    ss >> str;
    return str;
}
std::string CodeInsert::DoubleToString(double x)
{
    std::stringstream ss;
    string str;
    ss << x;
    ss >> str;
    return str;
}

std::vector<double> CodeInsert::Split(char * data)
{
    int i = 0;
    std::string argument;
    std::vector<double> argumentVector;
    while (true)
    {
        if (data[i] != ','&&data[i]!='\0')
        {
            argument += data[i];
            i++;
        }
        else if(data[i]==',')
        {
            i++;
            argumentVector.push_back(std::stod(argument));
            argument = "";
        }
        else if(data[i]=='\0')
        {
            argumentVector.push_back(std::stod(argument));
            break;
        }
    }
    return argumentVector;
}

const std::string CodeInsert::NodeTable()
{
    //using std::vector;
    //vector<int>::iterator it;
    int counter = 0;
    //for (it = nodeStatus.begin(); it < nodeStatus.end(); it++)
    //{
    //	if (*it == 1 || *it == 3)
    //	{
    //		counter++;//判断节点计数器
    //	}
    //}
    //char buffer[20];
    //_itoa_s(m_iConditionalNode, buffer, 10);
    //std::string s(buffer);
    std::stringstream ss;
    string s_temp;
    ss << m_iConditionalNode;
    ss >> s_temp;
    //return str;
    std::string nodeTable = "bool nodeTable[";
    nodeTable += s_temp;


    nodeTable.append("][2];");
    return nodeTable;
}



//插入代码有误if-else
//switch语句中case后面需要插入越来越多的条件执行语句
//匹配ENUM_KEYWORD::IF ENUM_KEYWORD::WHILE ENUM_KEYWORD::FOR ENUM_KEYWORD::SWITCH语句
void CodeInsert::BuildCFG(int rootVertex,bool isElseCall, int condition_node, bool is_while_call, int while_condition_node)
{

    int temp_condition_node = 0;  //因为在递归过程中可能会将m_conditionNode改变，所以创建这一个变量用以储存
    string switch_condition = "";
    int temp_while_condition_node_num = 0;
    //m_ssTestFunctionInsertCode
    //m_ssFitnessCalculateFunctionCode
    //m_fEmitCalculateFitnessFunction.calcuF(lexer.getString(lexer.getIndex() - 1), "tempFitness", m_iConditionalNode - 1);
    //lexer.getString(index)index是指要截取字符串的下一个
    //记录当前的嵌套语句是哪一种
    static std::stack<int> inWhichBlock;
    static std::vector<int> breakVertexes;
    //大括号计数变量
    int numOfBrace = 0;
    //小括号计数变量
    int numOfParenthese = 0;
    //swtich的case调用buildCFG时可能没有｛｝匹配，此时用break跳出递归

    if (m_classLexer.GetToken() == '{')
    {
        numOfBrace++;
    }
    //terminateVertex 是指语句终止节点，例如普通的if语句
    //   0
    //  / \
	// 1   2
    //  \ /
    //   3
    // 3便是终止节点， newYesVertex指的是 1 节点， newNoVertex指的是 2 节点
    int terminateVertex = 0, newYesVertex = 0, newNoVertex = 0;
    //while循环的控制流图：
    //     0
    //   / | \
	//   \ |  \
	//     1  /
    //       /
    //     2
    //0是loopJudgeVertex,1是loopBodyVertex,2是loopEndVertex
    //有两条路径0-1-0循环和0-2跳出循环
    int loopJudgeVertex = 0, loopBodyVertex = 0, loopEndVertex = 0;
    do {
        while (m_classLexer.GetToken() != ENUM_KEYWORD::DO&&m_classLexer.GetToken() != ENUM_KEYWORD::IF && m_classLexer.GetToken() != ENUM_KEYWORD::ELSE && m_classLexer.GetToken() != ENUM_KEYWORD::WHILE
            && m_classLexer.GetToken() != ENUM_KEYWORD::FOR && m_classLexer.GetToken() != ENUM_KEYWORD::SWITCH && m_classLexer.GetToken() != ENUM_KEYWORD::BREAK && m_classLexer.GetToken() != '}')
            m_classLexer.Match();
        switch (m_classLexer.GetToken())
        {
        case ENUM_KEYWORD::SWITCH:
            inWhichBlock.push(ENUM_KEYWORD::SWITCH);
            m_classLexer.Match();//match switch ,这里就应该是匹配左(了
            //这里需要将条件匹配出来
            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex()); // 将...switch (进行输出

            while (m_classLexer.GetToken() != ')')
            {
                m_classLexer.Match();
            }
            //当前token指向')'， index 指向 )的下一个字符
            switch_condition = m_classLexer.GetString(m_classLexer.GetIndex() - 1);
            cout << "switch_condition: "<<switch_condition << endl;
            m_ssTestFunctionInsertCode << switch_condition;



            //m_classLexer.Match();//match ( 此处匹配变量
             //!!!
            //get the switch value
            //m_classLexer.Match(); // 此处匹配的是右）
            m_classLexer.Match();//match )??????????，这里匹配左花括号

            rootVertex = BuildCFGOfSwitch(rootVertex, switch_condition, is_while_call, while_condition_node);
            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetPrevTokenIndex());
            if (is_while_call)
            {
                m_ssTestFunctionInsertCode << "if ( flag_" << while_condition_node << ")" << endl;
                m_ssTestFunctionInsertCode << "{" << endl;
                m_ssTestFunctionInsertCode << "path += \"-" << rootVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << "}" << endl;
            }
            else
            {
                m_ssTestFunctionInsertCode << "path += \"-" << rootVertex << "\";" << endl;
            }
       
            inWhichBlock.pop();
            break;
        case ENUM_KEYWORD::IF:
            inWhichBlock.push(ENUM_KEYWORD::IF);
            newYesVertex = m_classCFG.AddVertex();
            m_classCFG.AddEdge(rootVertex, newYesVertex);
            m_classLexer.Match();



            m_iConditionalNode++;   //判断节点+1
            temp_condition_node = m_iConditionalNode;
            //开始匹配判断条件的（）
            numOfParenthese = 0;

            if (m_classLexer.GetToken() == '(')
            {
                //已经匹配到左括号,将左花括号之前的字符串截取输出
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex()) << endl;



                m_classLexer.Match();
                numOfParenthese++;//保证匹配到if的右圆括号
                while (numOfParenthese > 0)
                {
                    if (m_classLexer.GetToken() == '(')
                    {
                        numOfParenthese++;
                    }
                    else if (m_classLexer.GetToken() == ')')
                    {
                        
                        numOfParenthese--;
                        if (numOfParenthese == 0)
                        {
                        
                            m_fEmitCalculateFitnessFunction.calcuF(m_classLexer.GetString(m_classLexer.GetIndex() - 1), "tempFitness", m_iConditionalNode - 1);
                            m_ssTestFunctionInsertCode << m_fEmitCalculateFitnessFunction.getExpr();
                            //将if语句中condition替换成函数表达式
                            m_ssFitnessCalculateFunctionCode << m_fEmitCalculateFitnessFunction.getFunc() << endl;
                            //右括号输出完毕
                            while (m_classLexer.Match() != '{')
                            {
                                if (m_classLexer.GetToken() == FILEEOF)
                                {
                                    break;
                                }
                            }
                            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
                            //代码插入部分
                            //最后一个右花括号，将判断表达式进行转换输出
                            //判断是否是while里面的while
                            if (is_while_call)
                            {
                                m_ssTestFunctionInsertCode << "if( flag_" << while_condition_node << ") " << endl;
                                m_ssTestFunctionInsertCode << "{" << endl;
                                m_ssTestFunctionInsertCode << "	globalFitness += tempFitness;tempFitness = 0; " << endl;
                                m_ssTestFunctionInsertCode << "nodeTable[" << m_iConditionalNode - 1 << "][0] = true;" << endl;
                                m_ssTestFunctionInsertCode << "path += \"-" << newYesVertex << "\";" << endl;
                                m_ssTestFunctionInsertCode << "}" << endl;
                            }
                            else
                            {
                                m_ssTestFunctionInsertCode << "	globalFitness += tempFitness;tempFitness = 0; " << endl;
                                m_ssTestFunctionInsertCode << "nodeTable[" << m_iConditionalNode - 1 << "][0] = true;" << endl;
                                m_ssTestFunctionInsertCode << "path += \"-" << newYesVertex << "\";" << endl;
                            }
                      

                            break;
                        }
                    }
                    m_classLexer.Match();
                }
                //在这里能够保证刚好匹配到左{
            }

            BuildCFG(newYesVertex,isElseCall, condition_node, is_while_call, while_condition_node);
            //处理if 紧跟的else的问题
            if (m_classLexer.GetToken() == ENUM_KEYWORD::ELSE)
            {
                while (m_classLexer.Match() != '{')
                {
                    if (m_classLexer.GetToken() == FILEEOF)
                    {
                        break;
                    }
                }
               // numOfBrace++;
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex()) << endl;
                //else 语句到左花括号输出
                newNoVertex = m_classCFG.AddVertex();
                m_classCFG.AddEdge(rootVertex, newNoVertex);

                if (is_while_call)
                {
                    m_ssTestFunctionInsertCode << "if ( flag_" << while_condition_node << ")" << endl;
                    m_ssTestFunctionInsertCode << "{" << endl;
                    m_ssTestFunctionInsertCode << "	globalFitness += tempFitness;tempFitness = 0; " << endl;
                    m_ssTestFunctionInsertCode << "nodeTable[" << temp_condition_node - 1 << "][1] = true;" << endl;
                    m_ssTestFunctionInsertCode << "path += \"-" << newNoVertex << "\";" << endl;
                    m_ssTestFunctionInsertCode << "}" << endl;
                }
                else
                {
                    m_ssTestFunctionInsertCode << "	globalFitness += tempFitness;tempFitness = 0; " << endl;
                    m_ssTestFunctionInsertCode << "nodeTable[" << temp_condition_node - 1 << "][1] = true;" << endl;
                    m_ssTestFunctionInsertCode << "path += \"-" << newNoVertex << "\";" << endl;
                }
               
                BuildCFG(newNoVertex);//因为什么条件退出
                //m_classLexer.Match();
                //index 指向花括号后的下一个token，如何不改动源代码的情况下，实现截取
                terminateVertex = m_classCFG.AddVertex();
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetPrevTokenIndex());
                if (is_while_call)
                {
                    m_ssTestFunctionInsertCode << "if ( flag_" << while_condition_node << ")" << endl;
                    m_ssTestFunctionInsertCode << "{" << endl;
                    m_ssTestFunctionInsertCode << "path += \"-" << terminateVertex << "\";" << endl;
                    m_ssTestFunctionInsertCode << "}" << endl;
                }
                else
                {
                    m_ssTestFunctionInsertCode << "path += \"-" << terminateVertex << "\";" << endl;
                }
              

                //m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
                if (!IsBreakVertex(breakVertexes, newYesVertex))
                    m_classCFG.AddEdge(m_classCFG.ToGround(newYesVertex), terminateVertex);
                if (!IsBreakVertex(breakVertexes, newNoVertex))
                    m_classCFG.AddEdge(m_classCFG.ToGround(newNoVertex), terminateVertex);
                rootVertex = terminateVertex;
            }
            inWhichBlock.pop();
            break;
        case ENUM_KEYWORD::DO:
        //处理的是do{}while()语句
            inWhichBlock.push(ENUM_KEYWORD::WHILE);
            loopBodyVertex = m_classCFG.AddVertex();
            m_iConditionalNode++;
            m_classCFG.AddEdge(rootVertex, loopBodyVertex);
            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetPrevTokenIndex());
            //声明循环变脸使用
            if (is_while_call)
            {
                m_ssTestFunctionInsertCode << "bool flag_" << m_iConditionalNode << " = true;" << endl;
                m_ssTestFunctionInsertCode << "bool flag_while_condition_node_" << m_iConditionalNode << " = false;" << endl;
                //m_ssTestFunctionInsertCode << "if ( flag_" << while_condition_node << ") {" << endl;

                //添加路径
               // m_ssTestFunctionInsertCode << "path += \"-" << loopBodyVertex << "\";" << endl;
              //  m_ssTestFunctionInsertCode << "}" << endl;
                //截取输出字符串到while
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
            }
            else
            {
                m_ssTestFunctionInsertCode << "bool flag_" << m_iConditionalNode << " = true;" << endl;
                m_ssTestFunctionInsertCode << "bool flag_while_condition_node_" << m_iConditionalNode << " = false;" << endl;
                //添加路径
                //m_ssTestFunctionInsertCode << "path += \"-" << loopBodyVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
            }

            //+++++++++++++++++++++++
            //进入dowhile循环体
            if (m_classLexer.Match() == '{')
            {
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
                m_ssTestFunctionInsertCode << "if (flag_" << m_iConditionalNode << "){" << endl;
                m_ssTestFunctionInsertCode << "	globalFitness += tempFitness;tempFitness = 0; " << endl;
                m_ssTestFunctionInsertCode << "nodeTable[" << m_iConditionalNode - 1 << "][0] = true;" << endl;
                m_ssTestFunctionInsertCode << "path += \"-" << loopBodyVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << "}" << endl;

                m_ssTestFunctionInsertCode << "if( flag_while_condition_node_" << m_iConditionalNode << ") {" << endl;
                //为什么减一，因为while回退的是while的头节点而不是body节点
                m_ssTestFunctionInsertCode << "path += \"-" << loopBodyVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << "flag_while_condition_node_" << m_iConditionalNode << "= false;" << endl;
                m_ssTestFunctionInsertCode << "}" << endl;
                temp_while_condition_node_num = m_iConditionalNode;
                BuildCFG(loopBodyVertex, false, 0, true, m_iConditionalNode);
                //现在应该指向while
                //while判断节点
                loopJudgeVertex = m_classCFG.AddVertex();
                m_classCFG.AddEdge(m_classCFG.ToGround(loopBodyVertex), loopJudgeVertex);
                m_classCFG.AddEdge(loopJudgeVertex,loopBodyVertex);

                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetPrevTokenIndex() - 1);
               
                m_ssTestFunctionInsertCode << "if (flag_" << temp_while_condition_node_num << "){" << endl;
                m_ssTestFunctionInsertCode << "path += \"-" << loopJudgeVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << "}" << endl;


                numOfParenthese = 0;
                if (m_classLexer.GetToken() == ENUM_KEYWORD::WHILE)
                {
                    if (m_classLexer.Match() == '(')
                    {
                        numOfParenthese++;
                        m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
                        m_classLexer.Match();
                        while (numOfParenthese > 0)
                        {
                            if (m_classLexer.GetToken() == '(')
                            {
                                numOfParenthese++;
                            }
                            else if (m_classLexer.GetToken() == ')')
                            {
                                numOfParenthese--;
                                if (numOfParenthese == 0)
                                {
                                    //最后一个右花括号，将判断表达式进行转换输出
                                    m_fEmitCalculateFitnessFunction.calcuF(m_classLexer.GetString(m_classLexer.GetIndex() - 1), "tempFitness", temp_while_condition_node_num - 1);
                                    m_ssTestFunctionInsertCode << m_fEmitCalculateFitnessFunction.getExpr();
                                    //将if语句中condition替换成函数表达式
                                    m_ssFitnessCalculateFunctionCode << m_fEmitCalculateFitnessFunction.getFunc() << endl;
                                    break;
                                }
                            }
                            else
                            {
                                m_classLexer.Match();
                            }
                        }
                        //while右小括号输出完毕
                        m_classLexer.Match(); //匹配;
                        m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());


                      
                        loopEndVertex = m_classCFG.AddVertex();
                        m_classCFG.AddEdge(loopJudgeVertex, loopEndVertex);
                        if (is_while_call)
                        {
                            m_ssTestFunctionInsertCode << "if ( flag_" << while_condition_node << ") {" << endl;
                            m_ssTestFunctionInsertCode << "path += \"-" << loopEndVertex << "\";" << endl;
                            m_ssTestFunctionInsertCode << "}" << endl;
                        }
                        else
                        {
                            m_ssTestFunctionInsertCode << "path += \"-" << loopEndVertex << "\";" << endl;
                        }


                    }
                    else
                    {
                        std::cerr << "while have no ("<< std::endl;
                    }
                    

                }
                else
                {
                    std::cerr << "ERROR in the do while statement!!!" << std::endl;
                }

            }
            //++++++++++++++++++++
            for (auto it = breakVertexes.begin(); it != breakVertexes.end(); it++)
                m_classCFG.AddEdge(*it, loopEndVertex);
            rootVertex = loopEndVertex;
            breakVertexes.clear();
            inWhichBlock.pop();

            break;


        case ENUM_KEYWORD::WHILE:
   
            inWhichBlock.push(ENUM_KEYWORD::WHILE);
    
            loopJudgeVertex = m_classCFG.AddVertex();
            m_classCFG.AddEdge(rootVertex, loopJudgeVertex);


            //这里需要插入一个临时变量，判断是否是while第一次执行
            //临时添加，可修改
            //++++++++++++++++++++++++++++++++++++++++
            //现在指向的while token，应该指向前一个字符，并进行插入
            m_iConditionalNode++;   //新的判断节点
            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetPrevTokenIndex()) << endl;
            if (is_while_call)
            {
                m_ssTestFunctionInsertCode << "bool flag_" << m_iConditionalNode << " = true;" << endl;
                m_ssTestFunctionInsertCode << "bool flag_while_condition_node_" << m_iConditionalNode << " = false;" << endl;
                m_ssTestFunctionInsertCode << "if ( flag_" << while_condition_node << ") {" << endl;
             
                //添加路径
                m_ssTestFunctionInsertCode << "path += \"-" << loopJudgeVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << "}" << endl;
                //截取输出字符串到while
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
            }
            else
            {
                m_ssTestFunctionInsertCode << "bool flag_" << m_iConditionalNode << " = true;" << endl;
                m_ssTestFunctionInsertCode << "bool flag_while_condition_node_" << m_iConditionalNode << " = false;" << endl;
                //添加路径
                m_ssTestFunctionInsertCode << "path += \"-" << loopJudgeVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
            }
   

            //+++++++++++++++++++++++++++++++++++++++++


            //开始匹配判断条件的（）
            m_classLexer.Match();
            numOfParenthese = 0;
            if (m_classLexer.GetToken() == '(')
            {
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
                m_classLexer.Match();
                numOfParenthese++;//保证匹配到if的右圆括号
                while (numOfParenthese > 0)
                {
                    if (m_classLexer.GetToken() == '(')
                    {
                        numOfParenthese++;
                    }
                    else if (m_classLexer.GetToken() == ')')
                    {
                        numOfParenthese--;
                        if (numOfParenthese == 0)
                        {

                            //最后一个右花括号，将判断表达式进行转换输出
                            m_fEmitCalculateFitnessFunction.calcuF(m_classLexer.GetString(m_classLexer.GetIndex() - 1), "tempFitness", m_iConditionalNode - 1);
                            m_ssTestFunctionInsertCode << m_fEmitCalculateFitnessFunction.getExpr();
                            //将if语句中condition替换成函数表达式
                            m_ssFitnessCalculateFunctionCode << m_fEmitCalculateFitnessFunction.getFunc() << endl;
                            //右括号输出完毕
                            while (m_classLexer.Match() != '{')
                            {
                                if (m_classLexer.GetToken() == FILEEOF)
                                {
                                    break;
                                }
                            }
                            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
                            //代码插入部分
                           

                            break;
                         
                        }
                    }
                    m_classLexer.Match();
                }
            }

            loopBodyVertex = m_classCFG.AddVertex();
            m_classCFG.AddEdge(loopJudgeVertex, loopBodyVertex);
            
            //代码插入部分
            //增加多一个条件，就是是否是第一次检验
            m_ssTestFunctionInsertCode << "if (flag_" << m_iConditionalNode << "){" << endl;
            m_ssTestFunctionInsertCode << "	globalFitness += tempFitness;tempFitness = 0; " << endl;
            m_ssTestFunctionInsertCode << "nodeTable[" << m_iConditionalNode - 1 << "][0] = true;" << endl;
            m_ssTestFunctionInsertCode << "path += \"-" << loopBodyVertex << "\";" << endl;
            m_ssTestFunctionInsertCode << "}" << endl;

            m_ssTestFunctionInsertCode << "if( flag_while_condition_node_" << m_iConditionalNode << ") {" << endl;
            //为什么减一，因为while回退的是while的头节点而不是body节点
            m_ssTestFunctionInsertCode << "path += \"-" << loopBodyVertex - 1 << "\";" << endl;
            m_ssTestFunctionInsertCode << "flag_while_condition_node_" << m_iConditionalNode << "= false;" << endl;
            m_ssTestFunctionInsertCode << "}" << endl;
            //继续判断
            //++++++++++++++++++++++++++++++++++++++++++++++++++++
            //进入while的循环体，也就是while(){ code },code部分
            temp_while_condition_node_num = m_iConditionalNode;
            BuildCFG(loopBodyVertex,false, 0, true, m_iConditionalNode);
            //+++++++++++++++++++++++++++++++++++++++++++++++++++


            m_classCFG.AddEdge(m_classCFG.ToGround(loopBodyVertex), loopJudgeVertex);
            loopEndVertex = m_classCFG.AddVertex();
            m_classCFG.AddEdge(loopJudgeVertex, loopEndVertex);
            for (auto it = breakVertexes.begin(); it != breakVertexes.end(); it++)
                m_classCFG.AddEdge(*it, loopEndVertex);
            rootVertex = loopEndVertex;
            breakVertexes.clear();
            inWhichBlock.pop();
            //while结束
            //为什么这里要减一，原因是因为这个代码的终止条件我看得稀里糊涂的，减一就能解决问题，所以就减一了
            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetPrevTokenIndex()-1);
         

            m_ssTestFunctionInsertCode << "if (flag_"<< temp_while_condition_node_num <<") { flag_while_condition_node_" << temp_while_condition_node_num << " = true ;}" << endl;


            m_ssTestFunctionInsertCode << "flag_" << temp_while_condition_node_num << " = false ;" << endl;
            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetPrevTokenIndex());
            if (is_while_call)
            {
                m_ssTestFunctionInsertCode << "if (flag_" << while_condition_node << ") {" << endl;
                m_ssTestFunctionInsertCode << "path +=\"-" << loopEndVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << "}" << endl;
            }
            else
            {
                m_ssTestFunctionInsertCode << "path +=\"-" << loopEndVertex << "\";" << endl;
            }
    
            break;
        case ENUM_KEYWORD::FOR:
            //处理for语句
            //进入{时进入递归，同时置函数参数is_while_call 为 true
            inWhichBlock.push(ENUM_KEYWORD::WHILE);
            loopJudgeVertex = m_classCFG.AddVertex();
            m_classCFG.AddEdge(rootVertex, loopJudgeVertex);

            //这里需要插入一个临时变量，判断是否是while第一次执行
            //临时添加，可修改
            //++++++++++++++++++++++++++++++++++++++++
            //现在指向的while token，应该指向前一个字符，并进行插入
            m_iConditionalNode++;   //新的判断节点
            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetPrevTokenIndex()) << endl;
            if (is_while_call)
            {
                m_ssTestFunctionInsertCode << "bool flag_" << m_iConditionalNode << " = true;" << endl;
                m_ssTestFunctionInsertCode << "bool flag_while_condition_node_" << m_iConditionalNode << " = false;" << endl;
                m_ssTestFunctionInsertCode << "if ( flag_" << while_condition_node << ") {" << endl;

                //添加路径
                m_ssTestFunctionInsertCode << "path += \"-" << loopJudgeVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << "}" << endl;
                //截取输出字符串到while
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
            }
            else
            {
                m_ssTestFunctionInsertCode << "bool flag_" << m_iConditionalNode << " = true;" << endl;
                m_ssTestFunctionInsertCode << "bool flag_while_condition_node_" << m_iConditionalNode << " = false;" << endl;
                //添加路径
                m_ssTestFunctionInsertCode << "path += \"-" << loopJudgeVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
            }
            if (m_classLexer.Match() == '(')
            {
                //初始话条件
                while (m_classLexer.Match() != ';'){}
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
                //判断条件
                while (m_classLexer.Match() != ';')
                {

                }
                //m_iConditionalNode++;
                //减一的原因是不需要截取到；，仅仅只需截取到这个判断条件即可
                m_fEmitCalculateFitnessFunction.calcuF(m_classLexer.GetString(m_classLexer.GetIndex() - 1), "tempFitness", m_iConditionalNode - 1);
                m_ssTestFunctionInsertCode << m_fEmitCalculateFitnessFunction.getExpr();
                //将if语句中condition替换成函数表达式
                m_ssFitnessCalculateFunctionCode << m_fEmitCalculateFitnessFunction.getFunc() << endl;
                
                //将for语句匹配完
                while (m_classLexer.Match() != '{')
                {
                }
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex()) << endl;
            }
            else { std::cerr << "for statement is not correct!!!!!" << std::endl; }
            loopBodyVertex = m_classCFG.AddVertex();
            m_classCFG.AddEdge(loopJudgeVertex, loopBodyVertex);

            //代码插入部分
            //增加多一个条件，就是是否是第一次检验
            m_ssTestFunctionInsertCode << "if (flag_" << m_iConditionalNode << "){" << endl;
            m_ssTestFunctionInsertCode << "	globalFitness += tempFitness;tempFitness = 0; " << endl;
            m_ssTestFunctionInsertCode << "nodeTable[" << m_iConditionalNode - 1 << "][0] = true;" << endl;
            m_ssTestFunctionInsertCode << "path += \"-" << loopBodyVertex << "\";" << endl;
            m_ssTestFunctionInsertCode << "}" << endl;

            m_ssTestFunctionInsertCode << "if( flag_while_condition_node_" << m_iConditionalNode << ") {" << endl;
            //为什么减一，因为while回退的是while的头节点而不是body节点
            m_ssTestFunctionInsertCode << "path += \"-" << loopBodyVertex - 1 << "\";" << endl;
            m_ssTestFunctionInsertCode << "flag_while_condition_node_" << m_iConditionalNode << "= false;" << endl;
            m_ssTestFunctionInsertCode << "}" << endl;

            temp_while_condition_node_num = m_iConditionalNode;
            BuildCFG(loopBodyVertex, false, 0, true, m_iConditionalNode);




            m_classCFG.AddEdge(m_classCFG.ToGround(loopBodyVertex), loopJudgeVertex);
            loopEndVertex = m_classCFG.AddVertex();
            m_classCFG.AddEdge(loopJudgeVertex, loopEndVertex);
            for (auto it = breakVertexes.begin(); it != breakVertexes.end(); it++)
                m_classCFG.AddEdge(*it, loopEndVertex);
            rootVertex = loopEndVertex;
            breakVertexes.clear();
            inWhichBlock.pop();
            //while结束
            //为什么这里要减一，原因是因为这个代码的终止条件我看得稀里糊涂的，减一就能解决问题，所以就减一了
            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetPrevTokenIndex() - 1);


            m_ssTestFunctionInsertCode << "if (flag_" << temp_while_condition_node_num << ") { flag_while_condition_node_" << temp_while_condition_node_num << " = true ;}" << endl;


            m_ssTestFunctionInsertCode << "flag_" << temp_while_condition_node_num << " = false ;" << endl;
            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetPrevTokenIndex());
            if (is_while_call)
            {
                m_ssTestFunctionInsertCode << "if (flag_" << while_condition_node << ") {" << endl;
                m_ssTestFunctionInsertCode << "nodeTable[" << temp_while_condition_node_num - 1 << "][1] = true;" << endl;
                m_ssTestFunctionInsertCode << "path +=\"-" << loopEndVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << "}" << endl;
            }
            else
            {
                m_ssTestFunctionInsertCode << "nodeTable[" << temp_while_condition_node_num - 1 << "][1] = true;" << endl;
                m_ssTestFunctionInsertCode << "path +=\"-" << loopEndVertex << "\";" << endl;
            }


            break;








        case ENUM_KEYWORD::BREAK:
            if (inWhichBlock.top() == ENUM_KEYWORD::WHILE || inWhichBlock.top() == ENUM_KEYWORD::IF)
            {
                breakVertexes.push_back(rootVertex);
                m_classLexer.Match();
                break;
            }
            else if (inWhichBlock.top() == ENUM_KEYWORD::SWITCH)
            {
                m_classLexer.Match();
                if (numOfBrace == 0)//说明碰到了case对应的break，这时才返回，解决了switch的case的嵌套问题
                    return;
                else
                    break;
            }
            else//应该不会有人皮到在while或者switch外面加break
            {
                m_classLexer.Match();
                break;
            }
        case '}':
            numOfBrace--;
            m_classLexer.Match();
            break;

        default:
            break;
        }
    } while (numOfBrace > 0 || (inWhichBlock.empty() ? false : inWhichBlock.top() == ENUM_KEYWORD::SWITCH));
    //当不在switch语句块内通过大括号数量判断块是否匹配完成，当在switch语句的case块内通过匹配到对应的break跳出循环
    //当整个函数匹配完成后inWhichBlock为空，跳出循环

}

int CodeInsert::BuildCFGOfSwitch(int rootVertex, string switch_condition, bool is_while_call, int while_condition_node)
{
    using std::vector;
    vector<string> switch_compare_case_condition;  //记录switch与case的比较字符串结果
    vector<int> switch_compare_case_condition_node_num;
    string case_condition = "";
    m_classLexer.Match();//匹配{之后的第一个token
    int numOfBrace = 0;
    numOfBrace++;
    //swith控制流图:
    //        0
    //   /  / ...\
	//  1  2      n
    //  \  \      /
    //        x
    //0是rootVertex,1,2...n是caseVertex,其中最后一个n是default ,x在buildCFG函数中，成为新的rootVertex
    //会有n条路径0-1-x,0-2-x....0-n-x,
    int caseVertex = 0;

    std::vector<int> caseVertexes;
    while (numOfBrace > 0)
    {
        while (m_classLexer.GetToken() != ENUM_KEYWORD::CASE && m_classLexer.GetToken() != ENUM_KEYWORD::DEFAULT && m_classLexer.GetToken() != '}')
            m_classLexer.Match();
        switch (m_classLexer.GetToken())
        {
        case ENUM_KEYWORD::CASE:
            //将...case代码输出
          //  cout << "case: " << m_classLexer.GetTokenStringValue() << endl;
            m_iConditionalNode++;
            switch_compare_case_condition_node_num.push_back(m_iConditionalNode);
            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex()) ;

            while (m_classLexer.GetToken() != ':')
            {
                m_classLexer.Match();
            }
            //现在匹配到 ':'

            case_condition = m_classLexer.GetString(m_classLexer.GetIndex() - 1);
            m_ssTestFunctionInsertCode << case_condition << m_classLexer.GetString(m_classLexer.GetIndex()) << endl;

            m_classLexer.Match();//match 常量值
         
            //形成判断条件
            switch_compare_case_condition.push_back(switch_condition);
            switch_compare_case_condition[switch_compare_case_condition.size() - 1].append(" == ");
            switch_compare_case_condition[switch_compare_case_condition.size() - 1].append(case_condition);
            //构造发射函数
            m_fEmitCalculateFitnessFunction.calcuF(switch_compare_case_condition[switch_compare_case_condition.size() - 1], "tempFitness", m_iConditionalNode - 1);
            m_ssFitnessCalculateFunctionCode << m_fEmitCalculateFitnessFunction.getFunc() << endl;
            //获取判断条件
            
            //匹配完: 后
            //++++++++++++++++++++++++++++++++++++++++++++++++++++++
            if (is_while_call)
            {
                m_ssTestFunctionInsertCode << "if ( flag_" << while_condition_node << ")" << endl;
                m_ssTestFunctionInsertCode << "{" << endl;
            }
            else
            {
                //else 就什么都不做
            }
            //+++++++++++++++++++++++++++++++++++++++++++++++++++++++


            for (unsigned int i = 0; i < switch_compare_case_condition.size(); i++)
            {
                //cout << *it << endl;
                m_fEmitCalculateFitnessFunction.calcuF(switch_compare_case_condition[i], "tempFitness", switch_compare_case_condition_node_num[i]- 1);
                m_ssTestFunctionInsertCode << m_fEmitCalculateFitnessFunction.getExpr() << ";" <<endl;
                m_ssTestFunctionInsertCode << "	globalFitness += tempFitness;tempFitness = 0; " << endl;
                if (switch_compare_case_condition.size() > 1 && i < switch_compare_case_condition.size() - 1)
                {
                    m_ssTestFunctionInsertCode << "nodeTable[" << switch_compare_case_condition_node_num[i] - 1 << "][1] = true;" << endl;
                }
            }

   
         
            //!!!
            //get the csae value &&make the condition:switch value==case value
           // m_classLexer.Match();//match value

            //m_classLexer.Match();	//match :

            caseVertex = m_classCFG.AddVertex();
            m_ssTestFunctionInsertCode << "nodeTable[" << switch_compare_case_condition_node_num[switch_compare_case_condition_node_num.size() - 1]-1 << "][0] = true;" << endl;
            m_ssTestFunctionInsertCode << "path += \"-" << caseVertex << "\";" << endl;

            //+++++++++++++++++++++++++++++++++
            //补上判断while语句中if语句的右花括号
            if (is_while_call)
            {
                m_ssTestFunctionInsertCode << "}" << endl;
            }
            else
            {

            }

            //+++++++++++++++++++++++++++++++++



            m_classCFG.AddEdge(rootVertex, caseVertex);
            //calculate fitness
            BuildCFG(caseVertex);
            caseVertexes.push_back(m_classCFG.ToGround(caseVertex));
            break;

        case ENUM_KEYWORD::DEFAULT:


            m_classLexer.Match(); //匹配 ':'
            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
            
            //m_fEmitCalculateFitnessFunction.calcuF(switch_compare_case_condition[switch_compare_case_condition.size() - 1], "tempFitness", switch_compare_case_condition_node_num[i] - 1);
            //m_ssFitnessCalculateFunctionCode << m_fEmitCalculateFitnessFunction.getFunc() << endl;

            //++++++++++++++++++++++++++++++++++++++++++++++++++++++
            if (is_while_call)
            {
                m_ssTestFunctionInsertCode << "if ( flag_" << while_condition_node << ")" << endl;
                m_ssTestFunctionInsertCode << "{" << endl;
            }
            else
            { }
            //++++++++++++++++++++++++++++++++++++++++++++++++++++++



            for (unsigned int i = 0; i < switch_compare_case_condition.size(); i++)
            {
                //cout << *it << endl;
                m_fEmitCalculateFitnessFunction.calcuF(switch_compare_case_condition[i], "tempFitness", switch_compare_case_condition_node_num[i] - 1);
                m_ssTestFunctionInsertCode << m_fEmitCalculateFitnessFunction.getExpr() << ";" << endl;
                m_ssTestFunctionInsertCode << "	globalFitness += tempFitness;tempFitness = 0; " << endl;
                if (switch_compare_case_condition.size() > 1 && i < switch_compare_case_condition.size() )
                {
                    m_ssTestFunctionInsertCode << "nodeTable[" << switch_compare_case_condition_node_num[i] - 1 << "][1] = true;" << endl;
                }
            }
            //m_classLexer.Match();//match default
            //!!!
            //get the defaule value &&make the condition:switch value!=defaule value

            //m_classLexer.Match();//match :
            caseVertex = m_classCFG.AddVertex();
            m_classCFG.AddEdge(rootVertex, caseVertex);
            m_ssTestFunctionInsertCode << "path += \"-" << caseVertex << "\";" << endl;

            //++++++++++++++++++++++++++++++++++++++++++++++++++++++
            if (is_while_call)
            {
                m_ssTestFunctionInsertCode << "}" << endl;
            }
            else
            {
            }
            //++++++++++++++++++++++++++++++++++++++++++++++++++++++
            //calculate fitness
            BuildCFG(caseVertex);
            caseVertexes.push_back(m_classCFG.ToGround(caseVertex));
            break;
        case '}':
            numOfBrace--;
            m_classLexer.Match();
            break;
        default:
            break;
        }
    }
    int terminateVertex = m_classCFG.AddVertex();

    for (unsigned int i = 0; i < caseVertexes.size(); i++)
    {
        m_classCFG.AddEdge(caseVertexes[i], terminateVertex);
    }
    return terminateVertex;
}









bool CodeInsert::IsBreakVertex(const std::vector<int>& breakVertexes, int vertex)
{
    for (auto it = breakVertexes.begin(); it != breakVertexes.end(); it++)
    {
        if (*it == vertex)
            return true;
    }
    return false;
}




//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void CodeInsert::CalculatePathNum()
{
    m_iNumOfPath = 20;
    //m_iNumOfPath = m_classCFG.CalculatePath();
}
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

bool CodeInsert::IsDataType(int tokenType)
{
    if (tokenType ==ENUM_KEYWORD::INT || tokenType == ENUM_KEYWORD::BOOL || tokenType ==ENUM_KEYWORD::CHAR || tokenType == ENUM_KEYWORD::DOUBLE)
        return true;
    else
        return false;
}

std::string CodeInsert::Insert()
{
    using std::cerr;
    using std::cout;
    using std::endl;
    using std::stack;
    //判断函数参数个数
    std::string DataName;
    int DataType;
    //匹配函数左括号
    while (m_classLexer.Match() != '(');
    //m_classLexer.match();
    while (m_classLexer.GetToken() != ')')
    {
        DataType = m_classLexer.Match();
        m_classLexer.Match();
        //匹配ENUM_KEYWORD::ID
        DataName = m_classLexer.GetTokenStringValue();
        m_classLexer.Match();
        //匹配逗号或者)
        //HashTableOfData[DataName] = DataType;
        if (m_classLexer.GetToken() == ')')
        {
            m_iNumOfParameter++;
            m_classLexer.Match();
            break;
        }
        else if (m_classLexer.GetToken() == ',')
        {
            m_iNumOfParameter++;
        }
    }
    //函数参数匹配结束
    std::cout << "参数数量：" << m_iNumOfParameter << std::endl;
    m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
    m_ssTestFunctionInsertCode<< "path += \"" << "0" << "\";" << endl;
    //构造控制流图
    BuildCFG(m_classCFG.AddVertex());

    m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetStringLength());
    
    m_classCFG.PrintList();
    //计算有多少条路径，总路径数，包含逻辑有效与逻辑无效路径
    CalculatePathNum();

    std::cout << "路径数：" << m_iNumOfPath << std::endl;
    InsertCode();
    return m_sTargetFile;

}

void CodeInsert::InsertCode()
{
    using std::string;
    using std::vector;
    using std::cout;
    using std::cin;
    using std::endl;
    using std::stack;
    //文件输出
    std::ofstream fout;
    fout.open(m_sTargetFile);
    if (!fout)
    {
        cout << "ERROR in read file" << endl;
        exit(-1);
    }
    m_classLexer.BackToFileBegin();
    //插入到文件processedFile.cpp
    using std::endl;
    //输出头文件
    fout << k_m_sHfile << endl;
    //输出函数声明
    fout << k_m_sFunctionDefine << endl;
    //计算fitness的函数声明
    //有几个判断节点就应该有多少fitness计算函数
    //计算Fitness函数声明结束
    //fitness计算实际函数定义
    string* fitnessFunction = new string[m_iConditionalNode];
    //string* fitnessFunctionDefine = new string[m_iConditionalNode];
    //fitness计算实际函数定义结束
    //输出nodeTable
    fout << NodeTable() << endl;
    //输出pathTable
    fout << k_m_sPathTable << endl;
    //输出path变量
    fout << k_m_sPath << endl;
    //输出Fitness变量
    fout << k_m_sFitness << endl;

    fout << "double tempFitness = 0;" << endl;
    //输出全局变量
    fout << "const double K = 10;	\r\n"
        "const double EPSILON = 0.01;\r\n"
        "const int POPNUM = 50;\r\n"
        "const int MCN = 100000;\r\n"
        "const double PC = 0.2;\r\n	"
        "const double F = 0.5;\r\n"
        "const int RUN = 30;\r\n"
        "int cycle[RUN] = { 0 };\r\n"
        "double coverage[RUN] = { 0 };	\r\n"
        "double runTime[RUN] = { 0 };\r\n"
        "int caseNum[RUN] = { 0 };" << endl;
    fout << "const int NODENUM = " << m_iConditionalNode << ";" << endl;
    fout << "const int PATHNUM = " << m_iNumOfPath << ";" << endl;
    fout << "const int R =" << m_iNumOfParameter << ";" << endl;


    //fout << ""
    fout << m_ssFitnessCalculateFunctionCode.str() << endl;

    fout << m_ssTestFunctionInsertCode.str() << endl;


    //fout << testFunctionStream.str() << endl;
    fout << "int main(int argc, char* argv[]){ DEAlgorithm();}" << endl;
    double* lb = new double[m_iNumOfParameter];
    double* ub = new double[m_iNumOfParameter];
    for (int i = 0; i < m_iNumOfParameter; i++)
    {
        lb[i] = 0;
        ub[i] = 150;
    }
    DEAlgorithm(m_iConditionalNode, m_iNumOfPath, m_iNumOfParameter, lb, ub);
    fout << m_sDEAlgorithmString << endl;
    fout.close();


}



