#include "stdafx.h"
#include "FilePreProcess.h"




FilePreProcess::FilePreProcess(string & localSrc):m_sSrc(localSrc)
{
	m_sSrc = localSrc;//引用赋值
	m_iMaxSize = m_sSrc.size();
	m_iOldIndex = m_iIndex = 0;
	//初始化关键字
	using std::string;

	m_structKeyWordTable["for"] = ENUM_KEYWORD::FOR;
	m_structKeyWordTable["while"] = ENUM_KEYWORD::WHILE;
	m_structKeyWordTable["if"] = ENUM_KEYWORD::IF;
	m_structKeyWordTable["else"] = ENUM_KEYWORD::ELSE;
	m_structKeyWordTable["switch"] = ENUM_KEYWORD::SWITCH;
	m_structKeyWordTable["break"] = ENUM_KEYWORD::BREAK;
	m_structKeyWordTable["case"] = ENUM_KEYWORD::CASE;
	m_structKeyWordTable["default"] = ENUM_KEYWORD::DEFAULT;
	m_structKeyWordTable["true"] = ENUM_KEYWORD::TRUE;
	m_structKeyWordTable["false"] = ENUM_KEYWORD::FALSE;
	m_structKeyWordTable["void"] = ENUM_KEYWORD::VOID;
	m_structKeyWordTable["int"] =ENUM_KEYWORD::INT;
	m_structKeyWordTable["double"] = ENUM_KEYWORD::DOUBLE;
	m_structKeyWordTable["bool"] = ENUM_KEYWORD::BOOL;
	m_structKeyWordTable["char"] =ENUM_KEYWORD::CHAR;
    m_structKeyWordTable["do"] = ENUM_KEYWORD::DO;

}


FilePreProcess::~FilePreProcess()
{
}

string & FilePreProcess::GetSrc()
{
	return m_sSrc;
}

int FilePreProcess::GetToken()
{
	return m_iToken;
}

int FilePreProcess::GetIndex()
{
	return m_iIndex;
}

void FilePreProcess::InsertCode(int target_index,std::string code)
{
	m_sSrc.insert(target_index,code);
}

void FilePreProcess::InsertChar()
{
	stack<bool>inBlock;
	inBlock.push(true);
	//直至文件分析结束，这个栈才可以弹出
	//源代码文件在m_sSrc中
	m_iIndex = m_sSrc.find("if");

	//直接指向第一个if
	if (m_iIndex != m_sSrc.npos)
	{
		//代码中存在if 语句，那就同时存在有else if 的可能性
		while (m_iIndex < m_iMaxSize&&m_sSrc[m_iIndex] != '{')
		{
			m_iIndex++;
		}
		if (m_iIndex < m_iMaxSize)
		{
			inBlock.push(true);
			m_iIndex++;
		}
		else
		{
			std::cout << "ERROR have no {" << std::endl;
			return ;
		}
		//现在的inBlock压的是if的左{括号
		int counter = 0;
		int tempIndex = 0;
		while (!inBlock.empty())
		{
			Match();

			switch (m_iToken)
			{
			case ENUM_KEYWORD::IF:
				NestProcess(true);
				break;
			case ENUM_KEYWORD::ELSE:
				tempIndex = m_iIndex;
				Match();
				if (m_iToken == ENUM_KEYWORD::IF)
				{
					m_sSrc.insert(tempIndex, "{");//debug的时候需要注意
					m_iIndex++;
					counter++;
					//现在这个是else if 状态
					NestProcess(true);


				}
				else if (m_iToken == '{')
				{
					PutBack();//往回退
					NestProcess(false);
					//分析完之后m_iIndex应该指向的是'}';
					for (int i = 0; i < counter; i++)
					{
						m_sSrc.insert(m_iIndex, "}");
						m_iIndex++;
					}
					counter = 0;
				}
				break;
			case '{':
				inBlock.push(true);
				break;
			case '}':
				inBlock.pop();
				break;
			case ENUM_KEYWORD::FILEEOF:
				inBlock.pop();
				break;
				
			default:
				break;
			}
		}
	}
	else
	{
		//代码中没有if语句
		return;
	}
	// cout << endl;
	// cout << m_sSrc << endl;
	return ;

}

void FilePreProcess::IfElsePair()
{
	stack<bool> IEPair; //if else pair
	stack<bool> inBlock;
	bool flag = false;//这个flag仅仅只是为了满足while条件中的判断
	IEPair.push(1);
	while (!IEPair.empty())
	{
		if (!flag)
		{
			IEPair.pop();
			flag = true;
		}
		switch (m_iToken)
		{
		case ENUM_KEYWORD::IF:
			IEPair.push(1);
			IfElsePairHelp(true);
			Match();
			//if 语句没有左括号，证明是单语句，无嵌套，可能需要匹配else；
			if (m_iToken != '{')
			{
				PutBack();
				m_sSrc.insert(m_iIndex, "{");
				m_iIndex++;
				while (m_iToken != ';')
				{
					Match();
				}
				m_sSrc.insert(m_iIndex, "}");
				m_iIndex++;
				//检查是否有else
				Match();
				if (m_iToken != ENUM_KEYWORD::ELSE)
				{
					PutBack();
					m_sSrc.insert(m_iIndex, "else{}");
				}
				else
				{
					PutBack();
				}
			}
			else
			{
				inBlock.push(1);
				while (!inBlock.empty())
				{
					Match();
					switch (m_iToken)
					{
					case ENUM_KEYWORD::IF:
						IfElsePair();
						break;
					case '{':
						inBlock.push(1);
						break;
					case '}':
						inBlock.pop();
						if (inBlock.empty())
						{
							Match();
							if (m_iToken == ENUM_KEYWORD::ELSE)
							{
								PutBack();
							}
							else
							{
								PutBack();
								m_sSrc.insert(m_iIndex, "else{}");
								
							}
						}
						break;
					}
				
				}
				
			}
			break;
		case ENUM_KEYWORD::ELSE:
			Match();
            //这里就是else的单语句
			if (m_iToken != '{' && m_iToken != ENUM_KEYWORD::IF)
			{
				PutBack();
				m_sSrc.insert(m_iIndex, "{");
				m_iIndex++;
				while (m_iToken != ';')
				{
					Match();
				}
				m_sSrc.insert(m_iIndex, "}");
				m_iIndex++;
			}
			else if (m_iToken == ENUM_KEYWORD::IF)
			{
				//else if 情形的处理
				//putback();
				IfElsePairHelp(true);
				Match();
				//if 语句没有左括号，证明是单语句，无嵌套，可能需要匹配else；
				if (m_iToken != '{')
				{
					PutBack();
					m_sSrc.insert(m_iIndex, "{");
					m_iIndex++;
					while (m_iToken != ';')
					{
						Match();
					}
					m_sSrc.insert(m_iIndex, "}");
					m_iIndex++;
					//检查是否有else
					Match();
					if (m_iToken != ENUM_KEYWORD::ELSE)
					{
						PutBack();
						m_sSrc.insert(m_iIndex, "else{}");
					}
					else
					{
						PutBack();
					}
				}
				else
				{
					inBlock.push(1);
					while (!inBlock.empty())
					{
						Match();
						switch (m_iToken)
						{
						case ENUM_KEYWORD::IF:
							IfElsePair();
							break;
						case '{':
							inBlock.push(1);
							break;
						case '}':
							inBlock.pop();
							if (inBlock.empty())
							{
								Match();
								if (m_iToken == ENUM_KEYWORD::ELSE)
								{
									PutBack();
								}
								else
								{
									PutBack();
									m_sSrc.insert(m_iIndex, "else{}");

								}
							}
							break;
						}

					}

				}
				break;
			}
			//处理else {}
			else
			{
				inBlock.push(1);
				
				while (!inBlock.empty())
				{
					Match();
					switch (m_iToken)
					{
					case ENUM_KEYWORD::IF:
						IfElsePair();
						break;
					case '{':
						inBlock.push(1);
						break;
					case '}':
						inBlock.pop();
						break;
					}
			
				}
			}
			IEPair.pop();
			break;
		

		}
		Match();
	}
	PutBack();
	return;
}
//将if语句中的圆括号匹配掉
void FilePreProcess::IfElsePairHelp( bool isIfCall)
{
	if (isIfCall)
	{
		stack<bool> brackets;
		Match();
		if (m_iToken == '(')
		{
			brackets.push(1);
		}
		else
		{
			cout << "ERROR if have no (" << endl;
		}
		while (!brackets.empty())
		{
			Match();
			switch (m_iToken)
			{
			case '(':
				brackets.push(1);
				break;
			case ')':
				brackets.pop();
				break;
			}
		}
		return;
	}
	else
	{

	}

}

void FilePreProcess::IfElsePairProgram()
{
	Match();
	while (m_iToken != ENUM_KEYWORD::FILEEOF)
	{
		switch (m_iToken)
		{
		case ENUM_KEYWORD::IF:
			IfElsePair();
			Match();
			break;
		default:
			Match();
		}
	}
}

void FilePreProcess::NestProcess(bool isIfCall)
{
	stack<bool> inBlock;
	if (isIfCall)
	{
		stack<bool>brackets;
		Match();
		if (m_iToken == '(')
		{
			brackets.push(1);
		}
		else
		{
			cout << " ERROR if  have no (" << endl;
		}
		while (!brackets.empty())
		{
			Match();
			switch (m_iToken)
			{
			case '(':
				brackets.push(1);
				break;
			case ')':
				brackets.pop();
				break;
			}
		}
		Match();

	}
	else
	{
		Match();
	}

	if (m_iToken == '{')
	{
		inBlock.push(true);
	}
	else
	{
		std::cerr << "ERROR in thr file preProcess" << endl;
	}
	int counter = 0;
	int tempIndex = 0;
	while (!inBlock.empty())
	{
		Match();

		switch (m_iToken)
		{
		case ENUM_KEYWORD::IF:
			NestProcess(true);
			break;
		case ENUM_KEYWORD::ELSE:
			tempIndex = m_iIndex;
			Match();
			if (m_iToken == ENUM_KEYWORD::IF)
			{
				m_sSrc.insert(tempIndex, "{");//debug的时候需要注意
				m_iIndex++;
				counter++;
				//现在这个是else if 状态
				NestProcess(true);
			}
			else if (m_iToken == '{')
			{
				PutBack();//往回退
				NestProcess(false);
				//分析完之后m_iIndex应该指向的是'}';
				for (int i = 0; i < counter; i++)
				{
					m_sSrc.insert(m_iIndex, "}");
					m_iIndex++;
				}
				counter = 0;
			}
			break;
		case '{':
			inBlock.push(true);
			break;
		case '}':
			inBlock.pop();
			break;


		case ENUM_KEYWORD::FILEEOF:
			//inBlock.pop();
			std::cerr << "ERROR in the file preProcess nest analysis" << endl;
			break;
		
		default:
			break;
			
		}
	}
}

void FilePreProcess::SwitchProgram()
{
	int numOfBrace = 0;
	std::vector<int> StartIndexes, EndIndexes;
	int BraceNumOfSwtich;
	while (GetToken() != ENUM_KEYWORD::FILEEOF)
	{
		if (GetToken() == ENUM_KEYWORD::SWITCH)
		{
			Match();
			while (GetToken() != '{') Match();
			Match();
			numOfBrace++;
			BraceNumOfSwtich = numOfBrace;
			while (numOfBrace >= BraceNumOfSwtich)
			{
				while (GetToken() != ENUM_KEYWORD::CASE && GetToken() != ENUM_KEYWORD::BREAK && GetToken() != '{'&& GetToken() != '}'&&GetToken() != ENUM_KEYWORD::DEFAULT)
					Match();
				if (GetToken() == ENUM_KEYWORD::CASE)
				{

					if (!StartIndexes.empty())//在匹配下一个case之前没有遇到一个break把StartIndexes清空
					{
						EndIndexes.push_back(GetIndex() - 5);//记录"case"之前一个字节的下标(GetIndex()得到case后一个字节' '的下标）
					}
					Match();//match "case"
					Match();//match case value

					StartIndexes.push_back(GetIndex());
					SwitchInBlock();
					continue;
				}
				else if (GetToken() == ENUM_KEYWORD::BREAK)
				{
					if (numOfBrace != BraceNumOfSwtich)//case里面有大括号，说明这个break应该是while里面的break而不是case匹配的break
					{
						Match();
						continue;
					}
					Match();//match ENUM_KEYWORD::BREAK
					if (StartIndexes.size() == 1)//表明一个case之后就有一个break
					{
						StartIndexes.clear();
						continue;
					}
					else if (StartIndexes.size() == 0)//deefault之后的break
					{
						Match();
						continue;
					}
					else
					{
						EndIndexes.push_back(GetIndex());//应该是要得到“break;”的;后面字节的下标，如果有需要要match ;
						for (int i = EndIndexes.size() - 2; i >= 0; i--)
						{
							//把StartIndexes[i+1]到EndIndexes[i+1]的代码插入到EndIndexes[i]的位置
							InsertCode(EndIndexes[i], m_sSrc.substr(StartIndexes[i + 1], EndIndexes[i + 1] - StartIndexes[i + 1]));
							//得到新的代码末尾的下标
							int lengthOfCode = EndIndexes[i + 1] - StartIndexes[i + 1];
							EndIndexes[i] += lengthOfCode;
							for (unsigned int j = i + 1; j < StartIndexes.size(); j++)
							{
								//因为代码插入了，后面的index都要相应位移这么多字节，方便最后继续扫描的时候用
								StartIndexes[j] += lengthOfCode;
								EndIndexes[j] += lengthOfCode;
							}
						}
						//这时候*EndIndexes.2end()记录的是最后一个break；后一个字节的下标，我们需要回到这里
						//，继续match，因为m_iToken一直没变，所以保留的一直是；后面的token(感觉是)
						m_iIndex = EndIndexes.back();
						StartIndexes.clear();
						EndIndexes.clear();
					}

				}
				else if (GetToken() == ENUM_KEYWORD::DEFAULT)
				{
					if (!StartIndexes.empty())//如果default之前的最后一个case没有break，我们给他加进去
					{
						InsertCode(GetIndex() - 9, "break;");//-9是为了回到default的前一个字符
						m_iIndex += 7;//加上break;的7个字符
						EndIndexes.push_back(GetIndex() - 9);//应该是要得到“break;”的;后面字节的下标，如果有需要要match ;
						for (int i = EndIndexes.size() - 2; i >= 0; i--)
						{
							//把StartIndexes[i+1]到EndIndexes[i+1]的代码插入到EndIndexes[i]的位置
							InsertCode(EndIndexes[i], m_sSrc.substr(StartIndexes[i + 1], EndIndexes[i + 1] - StartIndexes[i + 1]));
							//得到新的代码末尾的下标
							int lengthOfCode = EndIndexes[i + 1] - StartIndexes[i + 1];
							EndIndexes[i] += lengthOfCode;
							for (unsigned int j = i + 1; j < StartIndexes.size(); j++)
							{
								//因为代码插入了，后面的index都要相应位移这么多字节，方便最后继续扫描的时候用
								StartIndexes[j] += lengthOfCode;
								EndIndexes[j] += lengthOfCode;
							}
						}
						//这时候*EndIndexes.2end()记录的是最后一个break；后一个字节的下标，我们需要回到这里
						//，继续match，因为m_iToken一直没变，所以保留的一直是；后面的token(感觉是)
						m_iIndex = EndIndexes.back();
						StartIndexes.clear();
						EndIndexes.clear();
					}
					else
					{
						Match();
					}

				}
				else if (GetToken() == '}')
				{
					if (numOfBrace == BraceNumOfSwtich)
					{
						if (!StartIndexes.empty())//如果｝之前的没有碰到default而且最后一个case没有break，我们给他加进去
						{
							InsertCode(GetIndex() - 2, "break;");
							cout << m_sSrc;
							m_iIndex += 7;//加上break;的7个字符
							EndIndexes.push_back(GetIndex() - 2);
							for (int i = EndIndexes.size() - 2; i >= 0; i--)
							{
								//把StartIndexes[i+1]到EndIndexes[i+1]的代码插入到EndIndexes[i]的位置
								std::cout << m_sSrc;
								InsertCode(EndIndexes[i], m_sSrc.substr(StartIndexes[i + 1], EndIndexes[i + 1] - StartIndexes[i + 1]));
								std::cout << std::endl;
								std::cout << m_sSrc;
								//得到新的代码末尾的下标
								int lengthOfCode = EndIndexes[i + 1] - StartIndexes[i + 1];
								EndIndexes[i] += lengthOfCode;
								for (unsigned int j = i + 1; j < StartIndexes.size(); j++)
								{
									//因为代码插入了，后面的index都要相应位移这么多字节，方便最后继续扫描的时候用
									StartIndexes[j] += lengthOfCode;
									EndIndexes[j] += lengthOfCode;
								}
							}
							//这时候*EndIndexes.2end()记录的是最后一个break；后一个字节的下标，我们需要回到这里
							//，继续match，因为m_iToken一直没变，所以保留的一直是；后面的token(感觉是)
							m_iIndex = EndIndexes.back();
							StartIndexes.clear();
							EndIndexes.clear();
						}
						Match();
					}
					else
					{
						Match();
					}
					numOfBrace--;
				}
				else if (GetToken() == '{')
				{
					numOfBrace++;
					Match();
				}

			}
		}
		else if (GetToken() == '{')
		{
			numOfBrace++;
			Match();
		}
		else if (GetToken() == '}')
		{
			numOfBrace++;
			Match();
		}
		else
		{
			Match();
		}
	}
}

void FilePreProcess::SwitchInBlock()
{
	int numOfBrace = 0;
	std::vector<int> StartIndexes, EndIndexes;
	int BraceNumOfSwtich=1;
	while (GetToken() != ENUM_KEYWORD::SWITCH && GetToken() != ENUM_KEYWORD::CASE && GetToken() != ENUM_KEYWORD::BREAK
		&& GetToken() != ENUM_KEYWORD::DEFAULT &&GetToken()!='{'&& GetToken() != '}')
		Match();
	do
	{
		if (GetToken() == ENUM_KEYWORD::SWITCH)
		{
			Match();
			while (GetToken() != '{') Match();
			Match();
			numOfBrace++;
			BraceNumOfSwtich = numOfBrace;
			while (numOfBrace >= BraceNumOfSwtich)
			{
				while (GetToken() != ENUM_KEYWORD::CASE && GetToken() != ENUM_KEYWORD::BREAK && GetToken() != '{'&& GetToken() != '}'&&GetToken() != ENUM_KEYWORD::DEFAULT)
					Match();
				if (GetToken() == ENUM_KEYWORD::CASE)
				{

					if (!StartIndexes.empty())//在匹配下一个case之前没有遇到一个break把StartIndexes清空
					{
						EndIndexes.push_back(GetIndex() - 5);//记录"case"之前一个字节的下标(GetIndex()得到case后一个字节' '的下标）
					}
					Match();//match "case"
					Match();//match case value

					StartIndexes.push_back(GetIndex());
					SwitchInBlock();						  
					continue;
				}
				else if (GetToken() == ENUM_KEYWORD::BREAK)
				{
					if (numOfBrace != BraceNumOfSwtich)//case里面有大括号，说明这个break应该是while里面的break而不是case匹配的break
					{
						Match();
						continue;
					}
					Match();//match ENUM_KEYWORD::BREAK
					if (StartIndexes.size() == 1)//表明一个case之后就有一个break
					{
						StartIndexes.clear();
						continue;
					}
					else if (StartIndexes.size() == 0)//deefault之后的break
					{
						Match();
						continue;
					}
					else
					{
						EndIndexes.push_back(GetIndex());//应该是要得到“break;”的;后面字节的下标，如果有需要要match ;
						for (int i = EndIndexes.size() - 2; i >= 0; i--)
						{
							//把StartIndexes[i+1]到EndIndexes[i+1]的代码插入到EndIndexes[i]的位置
							InsertCode(EndIndexes[i], m_sSrc.substr(StartIndexes[i + 1], EndIndexes[i + 1] - StartIndexes[i + 1]));
							//得到新的代码末尾的下标
							int lengthOfCode = EndIndexes[i + 1] - StartIndexes[i + 1];
							EndIndexes[i] += lengthOfCode;
							for (unsigned int j = i + 1; j < StartIndexes.size(); j++)
							{
								//因为代码插入了，后面的index都要相应位移这么多字节，方便最后继续扫描的时候用
								StartIndexes[j] += lengthOfCode;
								EndIndexes[j] += lengthOfCode;
							}
						}
						//这时候*EndIndexes.2end()记录的是最后一个break；后一个字节的下标，我们需要回到这里
						//，继续match，因为m_iToken一直没变，所以保留的一直是；后面的token(感觉是)
						m_iIndex = EndIndexes.back();
						StartIndexes.clear();
						EndIndexes.clear();
					}

				}
				else if (GetToken() == ENUM_KEYWORD::DEFAULT)
				{
					if (!StartIndexes.empty())//如果default之前的最后一个case没有break，我们给他加进去
					{
						InsertCode(GetIndex() - 9, "break;");//-9是为了回到default的前一个字符
						m_iIndex += 7;//加上break;的7个字符
						EndIndexes.push_back(GetIndex() - 9);//应该是要得到“break;”的;后面字节的下标，如果有需要要match ;
						for (int i = EndIndexes.size() - 2; i >= 0; i--)
						{
							//把StartIndexes[i+1]到EndIndexes[i+1]的代码插入到EndIndexes[i]的位置
							InsertCode(EndIndexes[i], m_sSrc.substr(StartIndexes[i + 1], EndIndexes[i + 1] - StartIndexes[i + 1]));
							//得到新的代码末尾的下标
							int lengthOfCode = EndIndexes[i + 1] - StartIndexes[i + 1];
							EndIndexes[i] += lengthOfCode;
							for (unsigned int j = i + 1; j < StartIndexes.size(); j++)
							{
								//因为代码插入了，后面的index都要相应位移这么多字节，方便最后继续扫描的时候用
								StartIndexes[j] += lengthOfCode;
								EndIndexes[j] += lengthOfCode;
							}
						}
						//这时候*EndIndexes.2end()记录的是最后一个break；后一个字节的下标，我们需要回到这里
						//，继续match，因为m_iToken一直没变，所以保留的一直是；后面的token(感觉是)
						m_iIndex = EndIndexes.back();
						StartIndexes.clear();
						EndIndexes.clear();
					}
					else
					{
						Match();
					}

				}
				else if (GetToken() == '}')
				{
					if (numOfBrace == BraceNumOfSwtich)
					{
						if (!StartIndexes.empty())//如果｝之前的没有碰到default而且最后一个case没有break，我们给他加进去
						{
							InsertCode(GetIndex() - 2, "break;");
							cout << m_sSrc;
							m_iIndex += 7;//加上break;的7个字符
							EndIndexes.push_back(GetIndex() - 2);
							for (int i = EndIndexes.size() - 2; i >= 0; i--)
							{
								//把StartIndexes[i+1]到EndIndexes[i+1]的代码插入到EndIndexes[i]的位置
								std::cout << m_sSrc;
								InsertCode(EndIndexes[i], m_sSrc.substr(StartIndexes[i + 1], EndIndexes[i + 1] - StartIndexes[i + 1]));
								std::cout << std::endl;
								std::cout << m_sSrc;
								//得到新的代码末尾的下标
								int lengthOfCode = EndIndexes[i + 1] - StartIndexes[i + 1];
								EndIndexes[i] += lengthOfCode;
								for (unsigned int j = i + 1; j < StartIndexes.size(); j++)
								{
									//因为代码插入了，后面的index都要相应位移这么多字节，方便最后继续扫描的时候用
									StartIndexes[j] += lengthOfCode;
									EndIndexes[j] += lengthOfCode;
								}
							}
							//这时候*EndIndexes.2end()记录的是最后一个break；后一个字节的下标，我们需要回到这里
							//，继续match，因为m_iToken一直没变，所以保留的一直是；后面的token(感觉是)
							m_iIndex = EndIndexes.back();
							StartIndexes.clear();
							EndIndexes.clear();
						}
						Match();
					}
					else
					{
						Match();
					}
					numOfBrace--;
				}
				else if (GetToken() == '{')
				{
					numOfBrace++;
					Match();
				}

			}
		}
		else if (GetToken() == '{')
		{
			Match();
			numOfBrace++;
		}
		else if (GetToken() == '}')
		{
			if (numOfBrace == 0)
				return;
			else 
			{
				Match();
				numOfBrace--;
			}
		}
		else if (GetToken() == ENUM_KEYWORD::CASE || GetToken() == ENUM_KEYWORD::BREAK
			|| GetToken() == ENUM_KEYWORD::DEFAULT)
		{
			if (numOfBrace == 0)
				return;
			else
				Match();
		}
		else
		{
			Match();
		}
	}while (numOfBrace >= 0);
}

void FilePreProcess::WhileAndForProgram()
{
	int numOfParenthese = 0, numOfBrace = 0;
	while (GetToken() != ENUM_KEYWORD::FILEEOF)
	{
		while (GetToken() != ENUM_KEYWORD::DO && GetToken() != ENUM_KEYWORD::WHILE && GetToken() != ENUM_KEYWORD::FOR&&GetToken()!=ENUM_KEYWORD::FILEEOF)
			Match();
        if (GetToken() == ENUM_KEYWORD::FILEEOF)
        {
            return;
        }
        else if (GetToken() == ENUM_KEYWORD::DO)
        {
            //一个do匹配一个while
            while (GetToken() != ENUM_KEYWORD::WHILE)
            {
                Match();
            }


            Match();
            stack<bool>parenthese;
            if (GetToken() == '(')
            {
                parenthese.push(true);
            }
          
            while (!parenthese.empty())
            {
                Match();
                if (GetToken() == '(')
                {
                    parenthese.push(true);
                }
                else if (GetToken() == ')')
                {
                    parenthese.pop();
                }
            }
            continue;

        }
        else
        {
            Match();
        }
			
		if (GetToken() == '(')
		{
			numOfParenthese = 1;
			Match();
			while (numOfParenthese > 0)
			{
				if (GetToken() == '(')
				{
					numOfParenthese++;
				}
				else if (GetToken() == ')')
				{
					numOfParenthese--;
				}
				Match();
			}
		}
		if (GetToken() != '{')
		{
			PutBack();
			InsertCode(GetIndex(), "{");
			Match();//match )
			Match();//match {
			numOfBrace = 0;//这里初始化为0是因为要匹配下一个;或者在没有匹配到;就出现一个块
			//例如后面跟着if(){}
			while (GetToken() != ';'&&GetToken() != '{'&&GetToken()!='}')
				Match();
			switch (GetToken())
			{
			case ';':
					InsertCode(GetIndex(), "}");
					Match();//match }
				break;
			case '{':
				numOfBrace++;
				Match();
				while (numOfBrace > 0)
				{
					if (GetToken() == '{')
						numOfBrace++;
					else if (GetToken() == '}')
						numOfBrace--;
					Match();
				}
				PutBack();
				InsertCode(GetIndex(), "}");
				Match();
				Match();
				break;
			default:
				break;
			}


		}
	}
}

void FilePreProcess::BackToFileBegin()
{
	m_iIndex = 0;
	m_iToken = 0;
}

int FilePreProcess::Match()
{
	m_iMaxSize = m_sSrc.size();
	string tempName = "";
	m_iOldIndex = m_iIndex;//维护两个index 用以回退功能
	while (m_iIndex <= m_iMaxSize)
	{
		if ((m_sSrc[m_iIndex] >= 'a' && m_sSrc[m_iIndex] <= 'z') || (m_sSrc[m_iIndex] >= 'A' && m_sSrc[m_iIndex] <= 'Z') || m_sSrc[m_iIndex] == '_')
		{
			string name = "";
			name += m_sSrc[m_iIndex];
			m_iIndex++;
			while ((m_sSrc[m_iIndex] >= 'a' && m_sSrc[m_iIndex] <= 'z') || (m_sSrc[m_iIndex] >= 'A' && m_sSrc[m_iIndex] <= 'Z') || (m_sSrc[m_iIndex] >= '0' && m_sSrc[m_iIndex] <= '9') || m_sSrc[m_iIndex] == '_')
			{
				name += m_sSrc[m_iIndex];
				m_iIndex++;
			}
			//查询一下该字符串是否是关键字
			std::map<std::string, int>::iterator it;
			it = m_structKeyWordTable.find(name);
			if (it != m_structKeyWordTable.end())
			{
				//关键字
				//m_sCurrentTokenStringValue = name;
				m_iOldToken = m_iToken;
				m_iToken = it->second;
				//返回字符串的枚举值
				return m_iToken;
			}
			//当前变量是普通变量
			//m_sCurrentTokenStringValue = name;
            m_iOldToken = m_iToken;
			m_iToken = ENUM_KEYWORD::ID;
			return m_iToken;
			//这里变量的值与类型都将不在决定，而是在上一个分析模块里面定义
		}

		else if (m_sSrc[m_iIndex] >= '0'&& m_sSrc[m_iIndex] <= '9')
		{
			m_dTokenValue = m_sSrc[m_iIndex] - '0';
			m_iIndex++;
			while (m_sSrc[m_iIndex] >= '0'&& m_sSrc[m_iIndex] <= '9')
			{
				m_dTokenValue = m_dTokenValue * 10 + (m_sSrc[m_iIndex] - '0');
				m_iIndex++;
			}
			double base = 10;
			if (m_sSrc[m_iIndex] == '.')
			{
				//处理浮点数
				m_iIndex++;
				while (m_sSrc[m_iIndex] >= '0'&& m_sSrc[m_iIndex] <= '9')
				{
					m_dTokenValue += (m_sSrc[m_iIndex] - '0') / base;
					base *= 10;
					m_iIndex++;
				}
			}
			m_iOldToken = m_iToken;
			m_iToken =ENUM_KEYWORD::NUM;
			return m_iToken;
		}
		else if (m_sSrc[m_iIndex] == '*')
		{
			m_iIndex++;
			m_iOldToken = m_iToken;
			m_iToken = ENUM_KEYWORD::MUL;
			return m_iToken;
		}
		else if (m_sSrc[m_iIndex] == '/')
		{
			m_iIndex++;
			if (m_sSrc[m_iIndex] == '/')
			{
				m_iIndex++;
				while (m_sSrc[m_iIndex] != '\n')
				{
					m_iIndex++;//跳掉注释
				}
			}
			else
			{
				m_iOldToken = m_iToken;
				m_iToken = ENUM_KEYWORD::DIV;
				return m_iToken;
			}
		}
		else if (m_sSrc[m_iIndex] == '=')
		{
			m_iIndex++;
			if (m_sSrc[m_iIndex] == '=')
			{
				m_iIndex++;
				m_iOldToken = m_iToken;
				m_iToken = ENUM_KEYWORD::EQ;
				return m_iToken;

			}
			else
			{
				m_iOldToken = m_iToken;
				m_iToken = ENUM_KEYWORD::ASSIGN;
				return m_iToken;
			}
		}
		else if (m_sSrc[m_iIndex] == '+')
		{
			m_iIndex++;
			if (m_sSrc[m_iIndex] == '+')
			{
				m_iIndex++;
				m_iOldToken = m_iToken;
				m_iToken = ENUM_KEYWORD::INC;
				return m_iToken;
			}
			else
			{
				m_iOldToken = m_iToken;
				m_iToken = ENUM_KEYWORD::ADD;
				return m_iToken;
			}
		}
		else if (m_sSrc[m_iIndex] == '-')
		{
			m_iIndex++;
			if (m_sSrc[m_iIndex] == '-')
			{
				m_iIndex++;
				m_iOldToken = m_iToken;
				m_iToken = ENUM_KEYWORD::DEC;
				return m_iToken;
			}
			else
			{
				m_iOldToken = m_iToken;
				m_iToken = ENUM_KEYWORD::SUB;
				return m_iToken;
			}
		}
		else if (m_sSrc[m_iIndex] == '<')
		{
			m_iIndex++;
			if (m_sSrc[m_iIndex] == '=')
			{
				m_iIndex++;
				m_iOldToken = m_iToken;
				m_iToken = ENUM_KEYWORD::LE;
				return m_iToken;//<=
			}
			else
			{
				m_iOldToken = m_iToken;
				m_iToken = ENUM_KEYWORD::LT;
				return m_iToken;// <
			}
		}
		else if (m_sSrc[m_iIndex] == '>')
		{
			m_iIndex++;
			if (m_sSrc[m_iIndex] == '=')
			{
				m_iIndex++;
				m_iOldToken = m_iToken;
				m_iToken = ENUM_KEYWORD::GE;
				return m_iToken;//>=
			}
			else
			{
				m_iOldToken = m_iToken;
				m_iToken = ENUM_KEYWORD::GT;

				return m_iToken;//>
			}
		}
		else if (m_sSrc[m_iIndex] == '|')
		{
			m_iIndex++;
			if (m_sSrc[m_iIndex] == '|')
			{
				m_iIndex++;
				m_iOldToken = m_iToken;
				m_iToken = ENUM_KEYWORD::OR;
				return m_iToken;// '||'
			}
			else
			{
				m_iOldToken = m_iToken;
				m_iToken = ENUM_KEYWORD::LOR;

				return m_iToken;//  '|'
			}
		}
		else if (m_sSrc[m_iIndex] == '&')
		{
			m_iIndex++;
			if (m_sSrc[m_iIndex] == '&')
			{
				m_iIndex++;
				m_iOldToken = m_iToken;
				m_iToken = ENUM_KEYWORD::AND;
				return m_iToken;// '&&'
			}
			else
			{
				m_iOldToken = m_iToken;
				m_iToken = ENUM_KEYWORD::LAND;
				return m_iToken;//  '&&'
			}
		}
		else if (m_sSrc[m_iIndex] == ';' || m_sSrc[m_iIndex] == '{' || m_sSrc[m_iIndex] == '}' || m_sSrc[m_iIndex] == '(' || m_sSrc[m_iIndex] == ')' || m_sSrc[m_iIndex] == '[' || m_sSrc[m_iIndex] == ']' || m_sSrc[m_iIndex] == ',' || m_sSrc[m_iIndex] == ':')
		{
			m_iIndex++;
			m_iOldToken = m_iToken;
			m_iToken = m_sSrc[m_iIndex - 1];
			return m_iToken;
		}
		else
		{
			m_iIndex++;
		}

	}
	m_iToken = ENUM_KEYWORD::FILEEOF;//文件分析结束
	return m_iToken;
}

void FilePreProcess::PutBack()
{
	m_iIndex = m_iOldIndex;
	m_iToken = m_iOldToken;
}

void FilePreProcess::Program()
{
	//对while和for的预处理，一定要放在ifelse前面
	WhileAndForProgram();
	BackToFileBegin();
	//对ifelse的预处理
	IfElsePairProgram();
	BackToFileBegin();
	InsertChar();
	BackToFileBegin();
	cout << "\n经过预处理完后的代码：\n" << m_sSrc;
	//对switch的预处理
	SwitchProgram();
	BackToFileBegin();
	
	cout << "\n经过预处理完后的代码：\n"<<m_sSrc;
	return;
}
