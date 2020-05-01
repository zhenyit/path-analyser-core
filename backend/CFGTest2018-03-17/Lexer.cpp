#include "stdafx.h"
#include "Lexer.h"

Lexer::Lexer(std::string fileName)
{
	std::fstream input(fileName);
	InitKeyWordTable();//初始化关键字表
	if (!input)
	{
		std::cerr << "ERROR IN READ FILE!!!" << std::endl;
		exit(-1);
	}
	std::stringstream buffer;
	buffer << input.rdbuf();
	m_sSrc = buffer.str();
	m_iIndex = 0;	//指向当前字符串位置
	m_iLengthOfSrc = m_sSrc.size();	//当前字符串的长度
	m_dTokenValue = 0;
	m_iToken = -1;
	m_iLastIndex = 0;
    m_iPrevIndex = m_iIndex;
	

	FilePreProcess filePreProcess(m_sSrc);
	filePreProcess.Program();

	ChangeFunctionName();
	m_iIndex = 0;	//指向当前字符串位置
	m_iLengthOfSrc = m_sSrc.size();	//当前字符串的长度
	m_dTokenValue = 0;
	m_iToken = -1;
	m_iLastIndex = 0;
	//std::cout << m_sSrc;

}

Lexer::~Lexer()
{
}

void Lexer::InitKeyWordTable()
{
	using std::string;

	m_mapKeyWordTable["for"] = ENUM_KEYWORD::FOR;
	m_mapKeyWordTable["while"] = ENUM_KEYWORD::WHILE;
	m_mapKeyWordTable["if"] = ENUM_KEYWORD::IF;
	m_mapKeyWordTable["else"] = ENUM_KEYWORD::ELSE;
	m_mapKeyWordTable["switch"] = ENUM_KEYWORD::SWITCH;
	m_mapKeyWordTable["case"] = ENUM_KEYWORD::CASE;
	m_mapKeyWordTable["break"] = ENUM_KEYWORD::BREAK;
	m_mapKeyWordTable["default"] = ENUM_KEYWORD::DEFAULT;
	m_mapKeyWordTable["continue"] = ENUM_KEYWORD::CONTINUE;
	m_mapKeyWordTable["true"] = ENUM_KEYWORD::TRUE;
	m_mapKeyWordTable["false"] = ENUM_KEYWORD::FALSE;
	m_mapKeyWordTable["void"] = ENUM_KEYWORD::VOID;
	m_mapKeyWordTable["int"] =ENUM_KEYWORD::INT;
	m_mapKeyWordTable["double"] = ENUM_KEYWORD::DOUBLE;
	m_mapKeyWordTable["bool"] = ENUM_KEYWORD::BOOL;
	m_mapKeyWordTable["char"] =ENUM_KEYWORD::CHAR;
    m_mapKeyWordTable["do"] = ENUM_KEYWORD::DO;
}

void Lexer::ChangeFunctionName()
{

	bool firstTime = true;
	int tokenFirst;
	int indexFirst;
	int tokenSecond;
	int indexSecond;
	int tokenThird;
	int indexThird;
	while (true)
	{
		if (firstTime)
		{
			 tokenFirst = Match();
			 indexFirst = m_iIndex;
			 tokenSecond = Match();
			 indexSecond = m_iIndex;
			 tokenThird = Match();
			 indexThird = m_iIndex;
			 firstTime = false;
		}
		else
		{
			tokenFirst = tokenSecond;
			indexFirst = indexSecond;
			tokenSecond = tokenThird;
			indexSecond = indexThird;
			tokenThird = Match();
			indexThird = m_iIndex;
		}
		
		if ((tokenFirst ==ENUM_KEYWORD::INT || tokenFirst == ENUM_KEYWORD::DOUBLE || tokenFirst == ENUM_KEYWORD::BOOL || tokenFirst ==ENUM_KEYWORD::CHAR || tokenFirst == ENUM_KEYWORD::VOID) && tokenSecond == ENUM_KEYWORD::ID && tokenThird == '(')
		{
			m_sSrc.replace(indexFirst, indexSecond - indexFirst, " test_function");
			//现在识别出来是函数模块
			m_iIndex = 0;
			return;
		}
		else if (tokenFirst == ENUM_KEYWORD::FILEEOF || tokenSecond == ENUM_KEYWORD::FILEEOF)
		{
			std::cerr << "Error in the function parameter!!!!" << std::endl;
			//exit(-1);
			return;
			//没有找到正确的函数入口，直接退出
		}
	}
	return ;
}

int Lexer::Match()
{
	using std::string;
    m_iPrevIndex = m_iIndex;
	while (m_iIndex <= m_iLengthOfSrc)
	{
		//变量名关键字的识别代码
		if ((m_sSrc[m_iIndex] >= 'a' && m_sSrc[m_iIndex] <= 'z') || (m_sSrc[m_iIndex] >= 'A' && m_sSrc[m_iIndex] <= 'Z') || m_sSrc[m_iIndex] == '_')
		{
			string name = "";
			name += m_sSrc[m_iIndex];
			m_iIndex ++;
			while ((m_sSrc[m_iIndex] >= 'a' && m_sSrc[m_iIndex] <= 'z') || (m_sSrc[m_iIndex] >= 'A' && m_sSrc[m_iIndex] <= 'Z') || (m_sSrc[m_iIndex] >= '0' && m_sSrc[m_iIndex] <= '9') || m_sSrc[m_iIndex] == '_')
			{
				name += m_sSrc[m_iIndex];
				m_iIndex ++;
			}
			//查询一下该字符串是否是关键字
			std::map<std::string, int>::iterator it;
			it = m_mapKeyWordTable.find(name);
			if (it != m_mapKeyWordTable.end())
			{
				//关键字
				m_sCurrentTokenStringValue = name;
				m_iToken = it->second;
				//返回字符串的枚举值
				return it->second;
			}
			//当前变量是普通变量
			m_sCurrentTokenStringValue = name;
			m_iToken = ENUM_KEYWORD::ID;
			return ENUM_KEYWORD::ID;
			//这里变量的值与类型都将不在决定，而是在上一个分析模块里面定义
		}
		else if (m_sSrc[m_iIndex] >= '0'&& m_sSrc[m_iIndex] <= '9')
		{
			m_dTokenValue = m_sSrc[m_iIndex] - '0';
			m_iIndex ++;
			while (m_sSrc[m_iIndex] >= '0'&& m_sSrc[m_iIndex] <= '9')
			{
				m_dTokenValue = m_dTokenValue * 10 + (m_sSrc[m_iIndex] - '0');
				m_iIndex ++;
			}
			double base = 10;
			if (m_sSrc[m_iIndex] == '.')
			{
				//处理浮点数
				m_iIndex ++;
				while (m_sSrc[m_iIndex] >= '0'&& m_sSrc[m_iIndex] <= '9')
				{
					m_dTokenValue += (m_sSrc[m_iIndex] - '0') / base;
					base *= 10;
					m_iIndex ++;
				}
			}
			m_iToken =ENUM_KEYWORD::NUM;
			return ENUM_KEYWORD::NUM;
		}
		else if (m_sSrc[m_iIndex] == '*')
		{
			m_iIndex ++;
			m_iToken = ENUM_KEYWORD::MUL;
			return ENUM_KEYWORD::MUL;
		}
		else if (m_sSrc[m_iIndex] == '/')
		{
			m_iIndex ++;
			if (m_sSrc[m_iIndex] == '/')
			{
				m_iIndex ++;
				while (m_sSrc[m_iIndex] != '\n')
				{
					m_iIndex ++;//跳掉注释
				}
			}
			else
			{
				m_iToken = ENUM_KEYWORD::DIV;
				return ENUM_KEYWORD::DIV;
			}
		}
		else if (m_sSrc[m_iIndex] == '=')
		{
			m_iIndex ++;
			if (m_sSrc[m_iIndex] == '=')
			{
				m_iIndex ++;
				m_iToken = ENUM_KEYWORD::EQ;
				return ENUM_KEYWORD::EQ;

			}
			else
			{
				m_iToken = ENUM_KEYWORD::ASSIGN;
				return ENUM_KEYWORD::ASSIGN;
			}
		}
		else if (m_sSrc[m_iIndex] == '+')
		{
			m_iIndex ++;
			if (m_sSrc[m_iIndex] == '+')
			{
				m_iIndex ++;
				m_iToken = ENUM_KEYWORD::INC;
				return ENUM_KEYWORD::INC;
			}
			else
			{
				m_iToken = ENUM_KEYWORD::ADD;
				return ENUM_KEYWORD::ADD;
			}
		}
		else if (m_sSrc[m_iIndex] == '-')
		{
			m_iIndex ++;
			if (m_sSrc[m_iIndex] == '-')
			{
				m_iIndex ++;
				m_iToken = ENUM_KEYWORD::DEC;
				return ENUM_KEYWORD::DEC;
			}
			else
			{
				m_iToken = ENUM_KEYWORD::SUB;
				return ENUM_KEYWORD::SUB;
			}
		}
		else if (m_sSrc[m_iIndex] == '<')
		{
			m_iIndex ++;
			if (m_sSrc[m_iIndex] == '=')
			{
				m_iIndex ++;
				m_iToken = ENUM_KEYWORD::LE;
				return ENUM_KEYWORD::LE;//<=
			}
			else
			{
				m_iToken = ENUM_KEYWORD::LT;
				return ENUM_KEYWORD::LT;// <
			}
		}
		else if (m_sSrc[m_iIndex] == '>')
		{
			m_iIndex ++;
			if (m_sSrc[m_iIndex] == '=')
			{
				m_iIndex ++;
				m_iToken = ENUM_KEYWORD::GE;
				return ENUM_KEYWORD::GE;//>=
			}
			else
			{
				m_iToken = ENUM_KEYWORD::GT;
				return ENUM_KEYWORD::GT;//>
			}
		}
		else if (m_sSrc[m_iIndex] == '|')
		{
			m_iIndex ++;
			if (m_sSrc[m_iIndex] == '|')
			{
				m_iIndex ++;
				m_iToken = ENUM_KEYWORD::OR;
				return ENUM_KEYWORD::OR;// '||'
			}
			else
			{
				m_iToken = ENUM_KEYWORD::LOR;
				return ENUM_KEYWORD::LOR;//  '|'
			}
		}
		else if (m_sSrc[m_iIndex] == '&')
		{
			m_iIndex ++;
			if (m_sSrc[m_iIndex] == '&')
			{
				m_iIndex ++;
				m_iToken = ENUM_KEYWORD::AND;
				return ENUM_KEYWORD::AND;// '&&'
			}
			else
			{
				m_iToken = ENUM_KEYWORD::LAND;
				return ENUM_KEYWORD::LAND;//  '&&'
			}
		}
		else if (m_sSrc[m_iIndex] == ';' || m_sSrc[m_iIndex] == '{' || m_sSrc[m_iIndex] == '}' || m_sSrc[m_iIndex] == '(' || m_sSrc[m_iIndex] == ')' || m_sSrc[m_iIndex] == '[' || m_sSrc[m_iIndex] == ']' || m_sSrc[m_iIndex] == ',' || m_sSrc[m_iIndex] == ':')
		{
			m_iIndex ++;
			m_iToken = m_sSrc[m_iIndex  - 1];
			return m_sSrc[m_iIndex  - 1];
		}
		else
		{
			m_iIndex ++;
		}
	}
	m_iToken = ENUM_KEYWORD::FILEEOF;
	return ENUM_KEYWORD::FILEEOF;
	//表示文本已经分析结束，这个时候就返回-1；表示已经结束分析
}

void Lexer::BackToFileBegin()
{
	m_iIndex = 0;	//把扫描重置到字符串头
}

std::string Lexer::GetTokenStringValue()
{
	return m_sCurrentTokenStringValue;
}

int Lexer::GetToken() const
{
	return m_iToken;
}

double Lexer::GetTokenValue() const
{
	return m_dTokenValue;

}

int Lexer::GetIndex() const
{
	return m_iIndex;
}

std::string Lexer::GetString(int index)
{
	//index 下标指的是要截取字符串的下一个
	using std::string;
	string newSubString = m_sSrc.substr(m_iLastIndex, index - m_iLastIndex);
	m_iLastIndex = index;
	return newSubString;
}


void Lexer::BackToIndex(int index)
{
	m_iIndex = index;
	Match();
}

int Lexer::GetPrevTokenIndex() const
{
    return m_iPrevIndex;
}

long Lexer::GetStringLength() const
{
    return m_iLengthOfSrc;
}
