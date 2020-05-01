#include "stdafx.h"
#include "compilerCall.h"


CompilerCall::CompilerCall()
{
}

CompilerCall::CompilerCall(std::string file)
{
    m_sFileName = file;
}


CompilerCall::~CompilerCall()
{
}

void CompilerCall::call()
{
	std::string exeName = m_sFileName;
	exeName[m_sFileName.length() - 1] = 'e';
	exeName[m_sFileName.length() - 2] = 'x';
	exeName[m_sFileName.length() - 3] = 'e';
	
    std::string s = "g++ -o " + exeName +" "+ m_sFileName;

	system(s.c_str());


	std::string drawGraph = "dot CFG.gv -T png -o CFG.png";

    //����exe��ȡִ�н��
	system(exeName.c_str());
    
    //���ɶ�Ӧ��·��ͼ
	system(drawGraph.c_str());
}
