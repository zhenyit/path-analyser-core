#include "stdafx.h"
#include "CodeInsert.h"


//��������������ִ��
void CodeInsert::Program()
{
    //����processfile.cpp������processfile.exe
    Insert();
    compiler_call.call();
}


CodeInsert::CodeInsert(std::string sourceFileName, char* argv[])
    : m_classLexer(sourceFileName), compiler_call(m_sTargetFile)
{
    m_iNumOfParameter = 0;
    m_iNumOfPath = 1;
    m_iConditionalNode = 0;
    //����
    m_vectorArgumentBound.push_back(0);

}

CodeInsert::~CodeInsert() {}


//��ȡ�㷨Դ���룬��ͨ������������㷨Դ�������
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
    //����������Χ��ʼ��

    cout << "���½磺" << endl;
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
    //������Ҫ������Ժ������ô���
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
    //�������ý�����nodeTable��pathTable�Ѿ�����

    //���㵱ǰ���ڵڼ���path
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

    s = s + s1 + pathCalculate;//����������ʼ������ʼ����Ⱥ����
    s += "cycle[run] = 1;";
    //��Ⱥ���н�������
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
    //���������ޣ����к�������
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
    //�������ô������
    functionCallSecond += "globalFitness = 0;";
    //·���������
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
    //Fitness �������
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
    //�������Դ����
    sEvolution = sEvolution + fitnessCalculate + functionCallFirstTrue + functionCallSecondTrue + fitnessCompare + end;
    //���Ա������Դ����
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
    //		counter++;//�жϽڵ������
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



//�����������if-else
//switch�����case������Ҫ����Խ��Խ�������ִ�����
//ƥ��ENUM_KEYWORD::IF ENUM_KEYWORD::WHILE ENUM_KEYWORD::FOR ENUM_KEYWORD::SWITCH���
void CodeInsert::BuildCFG(int rootVertex,bool isElseCall, int condition_node, bool is_while_call, int while_condition_node)
{

    int temp_condition_node = 0;  //��Ϊ�ڵݹ�����п��ܻὫm_conditionNode�ı䣬���Դ�����һ���������Դ���
    string switch_condition = "";
    int temp_while_condition_node_num = 0;
    //m_ssTestFunctionInsertCode
    //m_ssFitnessCalculateFunctionCode
    //m_fEmitCalculateFitnessFunction.calcuF(lexer.getString(lexer.getIndex() - 1), "tempFitness", m_iConditionalNode - 1);
    //lexer.getString(index)index��ָҪ��ȡ�ַ�������һ��
    //��¼��ǰ��Ƕ���������һ��
    static std::stack<int> inWhichBlock;
    static std::vector<int> breakVertexes;
    //�����ż�������
    int numOfBrace = 0;
    //С���ż�������
    int numOfParenthese = 0;
    //swtich��case����buildCFGʱ����û�У���ƥ�䣬��ʱ��break�����ݹ�

    if (m_classLexer.GetToken() == '{')
    {
        numOfBrace++;
    }
    //terminateVertex ��ָ�����ֹ�ڵ㣬������ͨ��if���
    //   0
    //  / \
	// 1   2
    //  \ /
    //   3
    // 3������ֹ�ڵ㣬 newYesVertexָ���� 1 �ڵ㣬 newNoVertexָ���� 2 �ڵ�
    int terminateVertex = 0, newYesVertex = 0, newNoVertex = 0;
    //whileѭ���Ŀ�����ͼ��
    //     0
    //   / | \
	//   \ |  \
	//     1  /
    //       /
    //     2
    //0��loopJudgeVertex,1��loopBodyVertex,2��loopEndVertex
    //������·��0-1-0ѭ����0-2����ѭ��
    int loopJudgeVertex = 0, loopBodyVertex = 0, loopEndVertex = 0;
    do {
        while (m_classLexer.GetToken() != ENUM_KEYWORD::DO&&m_classLexer.GetToken() != ENUM_KEYWORD::IF && m_classLexer.GetToken() != ENUM_KEYWORD::ELSE && m_classLexer.GetToken() != ENUM_KEYWORD::WHILE
            && m_classLexer.GetToken() != ENUM_KEYWORD::FOR && m_classLexer.GetToken() != ENUM_KEYWORD::SWITCH && m_classLexer.GetToken() != ENUM_KEYWORD::BREAK && m_classLexer.GetToken() != '}')
            m_classLexer.Match();
        switch (m_classLexer.GetToken())
        {
        case ENUM_KEYWORD::SWITCH:
            inWhichBlock.push(ENUM_KEYWORD::SWITCH);
            m_classLexer.Match();//match switch ,�����Ӧ����ƥ����(��
            //������Ҫ������ƥ�����
            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex()); // ��...switch (�������

            while (m_classLexer.GetToken() != ')')
            {
                m_classLexer.Match();
            }
            //��ǰtokenָ��')'�� index ָ�� )����һ���ַ�
            switch_condition = m_classLexer.GetString(m_classLexer.GetIndex() - 1);
            cout << "switch_condition: "<<switch_condition << endl;
            m_ssTestFunctionInsertCode << switch_condition;



            //m_classLexer.Match();//match ( �˴�ƥ�����
             //!!!
            //get the switch value
            //m_classLexer.Match(); // �˴�ƥ������ң�
            m_classLexer.Match();//match )??????????������ƥ��������

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



            m_iConditionalNode++;   //�жϽڵ�+1
            temp_condition_node = m_iConditionalNode;
            //��ʼƥ���ж������ģ���
            numOfParenthese = 0;

            if (m_classLexer.GetToken() == '(')
            {
                //�Ѿ�ƥ�䵽������,��������֮ǰ���ַ�����ȡ���
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex()) << endl;



                m_classLexer.Match();
                numOfParenthese++;//��֤ƥ�䵽if����Բ����
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
                            //��if�����condition�滻�ɺ������ʽ
                            m_ssFitnessCalculateFunctionCode << m_fEmitCalculateFitnessFunction.getFunc() << endl;
                            //������������
                            while (m_classLexer.Match() != '{')
                            {
                                if (m_classLexer.GetToken() == FILEEOF)
                                {
                                    break;
                                }
                            }
                            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
                            //������벿��
                            //���һ���һ����ţ����жϱ��ʽ����ת�����
                            //�ж��Ƿ���while�����while
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
                //�������ܹ���֤�պ�ƥ�䵽��{
            }

            BuildCFG(newYesVertex,isElseCall, condition_node, is_while_call, while_condition_node);
            //����if ������else������
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
                //else ��䵽���������
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
               
                BuildCFG(newNoVertex);//��Ϊʲô�����˳�
                //m_classLexer.Match();
                //index ָ�����ź����һ��token����β��Ķ�Դ���������£�ʵ�ֽ�ȡ
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
        //�������do{}while()���
            inWhichBlock.push(ENUM_KEYWORD::WHILE);
            loopBodyVertex = m_classCFG.AddVertex();
            m_iConditionalNode++;
            m_classCFG.AddEdge(rootVertex, loopBodyVertex);
            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetPrevTokenIndex());
            //����ѭ������ʹ��
            if (is_while_call)
            {
                m_ssTestFunctionInsertCode << "bool flag_" << m_iConditionalNode << " = true;" << endl;
                m_ssTestFunctionInsertCode << "bool flag_while_condition_node_" << m_iConditionalNode << " = false;" << endl;
                //m_ssTestFunctionInsertCode << "if ( flag_" << while_condition_node << ") {" << endl;

                //���·��
               // m_ssTestFunctionInsertCode << "path += \"-" << loopBodyVertex << "\";" << endl;
              //  m_ssTestFunctionInsertCode << "}" << endl;
                //��ȡ����ַ�����while
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
            }
            else
            {
                m_ssTestFunctionInsertCode << "bool flag_" << m_iConditionalNode << " = true;" << endl;
                m_ssTestFunctionInsertCode << "bool flag_while_condition_node_" << m_iConditionalNode << " = false;" << endl;
                //���·��
                //m_ssTestFunctionInsertCode << "path += \"-" << loopBodyVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
            }

            //+++++++++++++++++++++++
            //����dowhileѭ����
            if (m_classLexer.Match() == '{')
            {
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
                m_ssTestFunctionInsertCode << "if (flag_" << m_iConditionalNode << "){" << endl;
                m_ssTestFunctionInsertCode << "	globalFitness += tempFitness;tempFitness = 0; " << endl;
                m_ssTestFunctionInsertCode << "nodeTable[" << m_iConditionalNode - 1 << "][0] = true;" << endl;
                m_ssTestFunctionInsertCode << "path += \"-" << loopBodyVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << "}" << endl;

                m_ssTestFunctionInsertCode << "if( flag_while_condition_node_" << m_iConditionalNode << ") {" << endl;
                //Ϊʲô��һ����Ϊwhile���˵���while��ͷ�ڵ������body�ڵ�
                m_ssTestFunctionInsertCode << "path += \"-" << loopBodyVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << "flag_while_condition_node_" << m_iConditionalNode << "= false;" << endl;
                m_ssTestFunctionInsertCode << "}" << endl;
                temp_while_condition_node_num = m_iConditionalNode;
                BuildCFG(loopBodyVertex, false, 0, true, m_iConditionalNode);
                //����Ӧ��ָ��while
                //while�жϽڵ�
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
                                    //���һ���һ����ţ����жϱ��ʽ����ת�����
                                    m_fEmitCalculateFitnessFunction.calcuF(m_classLexer.GetString(m_classLexer.GetIndex() - 1), "tempFitness", temp_while_condition_node_num - 1);
                                    m_ssTestFunctionInsertCode << m_fEmitCalculateFitnessFunction.getExpr();
                                    //��if�����condition�滻�ɺ������ʽ
                                    m_ssFitnessCalculateFunctionCode << m_fEmitCalculateFitnessFunction.getFunc() << endl;
                                    break;
                                }
                            }
                            else
                            {
                                m_classLexer.Match();
                            }
                        }
                        //while��С����������
                        m_classLexer.Match(); //ƥ��;
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


            //������Ҫ����һ����ʱ�������ж��Ƿ���while��һ��ִ��
            //��ʱ��ӣ����޸�
            //++++++++++++++++++++++++++++++++++++++++
            //����ָ���while token��Ӧ��ָ��ǰһ���ַ��������в���
            m_iConditionalNode++;   //�µ��жϽڵ�
            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetPrevTokenIndex()) << endl;
            if (is_while_call)
            {
                m_ssTestFunctionInsertCode << "bool flag_" << m_iConditionalNode << " = true;" << endl;
                m_ssTestFunctionInsertCode << "bool flag_while_condition_node_" << m_iConditionalNode << " = false;" << endl;
                m_ssTestFunctionInsertCode << "if ( flag_" << while_condition_node << ") {" << endl;
             
                //���·��
                m_ssTestFunctionInsertCode << "path += \"-" << loopJudgeVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << "}" << endl;
                //��ȡ����ַ�����while
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
            }
            else
            {
                m_ssTestFunctionInsertCode << "bool flag_" << m_iConditionalNode << " = true;" << endl;
                m_ssTestFunctionInsertCode << "bool flag_while_condition_node_" << m_iConditionalNode << " = false;" << endl;
                //���·��
                m_ssTestFunctionInsertCode << "path += \"-" << loopJudgeVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
            }
   

            //+++++++++++++++++++++++++++++++++++++++++


            //��ʼƥ���ж������ģ���
            m_classLexer.Match();
            numOfParenthese = 0;
            if (m_classLexer.GetToken() == '(')
            {
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
                m_classLexer.Match();
                numOfParenthese++;//��֤ƥ�䵽if����Բ����
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

                            //���һ���һ����ţ����жϱ��ʽ����ת�����
                            m_fEmitCalculateFitnessFunction.calcuF(m_classLexer.GetString(m_classLexer.GetIndex() - 1), "tempFitness", m_iConditionalNode - 1);
                            m_ssTestFunctionInsertCode << m_fEmitCalculateFitnessFunction.getExpr();
                            //��if�����condition�滻�ɺ������ʽ
                            m_ssFitnessCalculateFunctionCode << m_fEmitCalculateFitnessFunction.getFunc() << endl;
                            //������������
                            while (m_classLexer.Match() != '{')
                            {
                                if (m_classLexer.GetToken() == FILEEOF)
                                {
                                    break;
                                }
                            }
                            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
                            //������벿��
                           

                            break;
                         
                        }
                    }
                    m_classLexer.Match();
                }
            }

            loopBodyVertex = m_classCFG.AddVertex();
            m_classCFG.AddEdge(loopJudgeVertex, loopBodyVertex);
            
            //������벿��
            //���Ӷ�һ�������������Ƿ��ǵ�һ�μ���
            m_ssTestFunctionInsertCode << "if (flag_" << m_iConditionalNode << "){" << endl;
            m_ssTestFunctionInsertCode << "	globalFitness += tempFitness;tempFitness = 0; " << endl;
            m_ssTestFunctionInsertCode << "nodeTable[" << m_iConditionalNode - 1 << "][0] = true;" << endl;
            m_ssTestFunctionInsertCode << "path += \"-" << loopBodyVertex << "\";" << endl;
            m_ssTestFunctionInsertCode << "}" << endl;

            m_ssTestFunctionInsertCode << "if( flag_while_condition_node_" << m_iConditionalNode << ") {" << endl;
            //Ϊʲô��һ����Ϊwhile���˵���while��ͷ�ڵ������body�ڵ�
            m_ssTestFunctionInsertCode << "path += \"-" << loopBodyVertex - 1 << "\";" << endl;
            m_ssTestFunctionInsertCode << "flag_while_condition_node_" << m_iConditionalNode << "= false;" << endl;
            m_ssTestFunctionInsertCode << "}" << endl;
            //�����ж�
            //++++++++++++++++++++++++++++++++++++++++++++++++++++
            //����while��ѭ���壬Ҳ����while(){ code },code����
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
            //while����
            //Ϊʲô����Ҫ��һ��ԭ������Ϊ����������ֹ�����ҿ���ϡ���Ϳ�ģ���һ���ܽ�����⣬���Ծͼ�һ��
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
            //����for���
            //����{ʱ����ݹ飬ͬʱ�ú�������is_while_call Ϊ true
            inWhichBlock.push(ENUM_KEYWORD::WHILE);
            loopJudgeVertex = m_classCFG.AddVertex();
            m_classCFG.AddEdge(rootVertex, loopJudgeVertex);

            //������Ҫ����һ����ʱ�������ж��Ƿ���while��һ��ִ��
            //��ʱ��ӣ����޸�
            //++++++++++++++++++++++++++++++++++++++++
            //����ָ���while token��Ӧ��ָ��ǰһ���ַ��������в���
            m_iConditionalNode++;   //�µ��жϽڵ�
            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetPrevTokenIndex()) << endl;
            if (is_while_call)
            {
                m_ssTestFunctionInsertCode << "bool flag_" << m_iConditionalNode << " = true;" << endl;
                m_ssTestFunctionInsertCode << "bool flag_while_condition_node_" << m_iConditionalNode << " = false;" << endl;
                m_ssTestFunctionInsertCode << "if ( flag_" << while_condition_node << ") {" << endl;

                //���·��
                m_ssTestFunctionInsertCode << "path += \"-" << loopJudgeVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << "}" << endl;
                //��ȡ����ַ�����while
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
            }
            else
            {
                m_ssTestFunctionInsertCode << "bool flag_" << m_iConditionalNode << " = true;" << endl;
                m_ssTestFunctionInsertCode << "bool flag_while_condition_node_" << m_iConditionalNode << " = false;" << endl;
                //���·��
                m_ssTestFunctionInsertCode << "path += \"-" << loopJudgeVertex << "\";" << endl;
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
            }
            if (m_classLexer.Match() == '(')
            {
                //��ʼ������
                while (m_classLexer.Match() != ';'){}
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
                //�ж�����
                while (m_classLexer.Match() != ';')
                {

                }
                //m_iConditionalNode++;
                //��һ��ԭ���ǲ���Ҫ��ȡ����������ֻ���ȡ������ж���������
                m_fEmitCalculateFitnessFunction.calcuF(m_classLexer.GetString(m_classLexer.GetIndex() - 1), "tempFitness", m_iConditionalNode - 1);
                m_ssTestFunctionInsertCode << m_fEmitCalculateFitnessFunction.getExpr();
                //��if�����condition�滻�ɺ������ʽ
                m_ssFitnessCalculateFunctionCode << m_fEmitCalculateFitnessFunction.getFunc() << endl;
                
                //��for���ƥ����
                while (m_classLexer.Match() != '{')
                {
                }
                m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex()) << endl;
            }
            else { std::cerr << "for statement is not correct!!!!!" << std::endl; }
            loopBodyVertex = m_classCFG.AddVertex();
            m_classCFG.AddEdge(loopJudgeVertex, loopBodyVertex);

            //������벿��
            //���Ӷ�һ�������������Ƿ��ǵ�һ�μ���
            m_ssTestFunctionInsertCode << "if (flag_" << m_iConditionalNode << "){" << endl;
            m_ssTestFunctionInsertCode << "	globalFitness += tempFitness;tempFitness = 0; " << endl;
            m_ssTestFunctionInsertCode << "nodeTable[" << m_iConditionalNode - 1 << "][0] = true;" << endl;
            m_ssTestFunctionInsertCode << "path += \"-" << loopBodyVertex << "\";" << endl;
            m_ssTestFunctionInsertCode << "}" << endl;

            m_ssTestFunctionInsertCode << "if( flag_while_condition_node_" << m_iConditionalNode << ") {" << endl;
            //Ϊʲô��һ����Ϊwhile���˵���while��ͷ�ڵ������body�ڵ�
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
            //while����
            //Ϊʲô����Ҫ��һ��ԭ������Ϊ����������ֹ�����ҿ���ϡ���Ϳ�ģ���һ���ܽ�����⣬���Ծͼ�һ��
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
                if (numOfBrace == 0)//˵��������case��Ӧ��break����ʱ�ŷ��أ������switch��case��Ƕ������
                    return;
                else
                    break;
            }
            else//Ӧ�ò�������Ƥ����while����switch�����break
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
    //������switch������ͨ�������������жϿ��Ƿ�ƥ����ɣ�����switch����case����ͨ��ƥ�䵽��Ӧ��break����ѭ��
    //����������ƥ����ɺ�inWhichBlockΪ�գ�����ѭ��

}

int CodeInsert::BuildCFGOfSwitch(int rootVertex, string switch_condition, bool is_while_call, int while_condition_node)
{
    using std::vector;
    vector<string> switch_compare_case_condition;  //��¼switch��case�ıȽ��ַ������
    vector<int> switch_compare_case_condition_node_num;
    string case_condition = "";
    m_classLexer.Match();//ƥ��{֮��ĵ�һ��token
    int numOfBrace = 0;
    numOfBrace++;
    //swith������ͼ:
    //        0
    //   /  / ...\
	//  1  2      n
    //  \  \      /
    //        x
    //0��rootVertex,1,2...n��caseVertex,�������һ��n��default ,x��buildCFG�����У���Ϊ�µ�rootVertex
    //����n��·��0-1-x,0-2-x....0-n-x,
    int caseVertex = 0;

    std::vector<int> caseVertexes;
    while (numOfBrace > 0)
    {
        while (m_classLexer.GetToken() != ENUM_KEYWORD::CASE && m_classLexer.GetToken() != ENUM_KEYWORD::DEFAULT && m_classLexer.GetToken() != '}')
            m_classLexer.Match();
        switch (m_classLexer.GetToken())
        {
        case ENUM_KEYWORD::CASE:
            //��...case�������
          //  cout << "case: " << m_classLexer.GetTokenStringValue() << endl;
            m_iConditionalNode++;
            switch_compare_case_condition_node_num.push_back(m_iConditionalNode);
            m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex()) ;

            while (m_classLexer.GetToken() != ':')
            {
                m_classLexer.Match();
            }
            //����ƥ�䵽 ':'

            case_condition = m_classLexer.GetString(m_classLexer.GetIndex() - 1);
            m_ssTestFunctionInsertCode << case_condition << m_classLexer.GetString(m_classLexer.GetIndex()) << endl;

            m_classLexer.Match();//match ����ֵ
         
            //�γ��ж�����
            switch_compare_case_condition.push_back(switch_condition);
            switch_compare_case_condition[switch_compare_case_condition.size() - 1].append(" == ");
            switch_compare_case_condition[switch_compare_case_condition.size() - 1].append(case_condition);
            //���췢�亯��
            m_fEmitCalculateFitnessFunction.calcuF(switch_compare_case_condition[switch_compare_case_condition.size() - 1], "tempFitness", m_iConditionalNode - 1);
            m_ssFitnessCalculateFunctionCode << m_fEmitCalculateFitnessFunction.getFunc() << endl;
            //��ȡ�ж�����
            
            //ƥ����: ��
            //++++++++++++++++++++++++++++++++++++++++++++++++++++++
            if (is_while_call)
            {
                m_ssTestFunctionInsertCode << "if ( flag_" << while_condition_node << ")" << endl;
                m_ssTestFunctionInsertCode << "{" << endl;
            }
            else
            {
                //else ��ʲô������
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
            //�����ж�while�����if�����һ�����
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


            m_classLexer.Match(); //ƥ�� ':'
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
    //�жϺ�����������
    std::string DataName;
    int DataType;
    //ƥ�亯��������
    while (m_classLexer.Match() != '(');
    //m_classLexer.match();
    while (m_classLexer.GetToken() != ')')
    {
        DataType = m_classLexer.Match();
        m_classLexer.Match();
        //ƥ��ENUM_KEYWORD::ID
        DataName = m_classLexer.GetTokenStringValue();
        m_classLexer.Match();
        //ƥ�䶺�Ż���)
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
    //��������ƥ�����
    std::cout << "����������" << m_iNumOfParameter << std::endl;
    m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetIndex());
    m_ssTestFunctionInsertCode<< "path += \"" << "0" << "\";" << endl;
    //���������ͼ
    BuildCFG(m_classCFG.AddVertex());

    m_ssTestFunctionInsertCode << m_classLexer.GetString(m_classLexer.GetStringLength());
    
    m_classCFG.PrintList();
    //�����ж�����·������·�����������߼���Ч���߼���Ч·��
    CalculatePathNum();

    std::cout << "·������" << m_iNumOfPath << std::endl;
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
    //�ļ����
    std::ofstream fout;
    fout.open(m_sTargetFile);
    if (!fout)
    {
        cout << "ERROR in read file" << endl;
        exit(-1);
    }
    m_classLexer.BackToFileBegin();
    //���뵽�ļ�processedFile.cpp
    using std::endl;
    //���ͷ�ļ�
    fout << k_m_sHfile << endl;
    //�����������
    fout << k_m_sFunctionDefine << endl;
    //����fitness�ĺ�������
    //�м����жϽڵ��Ӧ���ж���fitness���㺯��
    //����Fitness������������
    //fitness����ʵ�ʺ�������
    string* fitnessFunction = new string[m_iConditionalNode];
    //string* fitnessFunctionDefine = new string[m_iConditionalNode];
    //fitness����ʵ�ʺ����������
    //���nodeTable
    fout << NodeTable() << endl;
    //���pathTable
    fout << k_m_sPathTable << endl;
    //���path����
    fout << k_m_sPath << endl;
    //���Fitness����
    fout << k_m_sFitness << endl;

    fout << "double tempFitness = 0;" << endl;
    //���ȫ�ֱ���
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



