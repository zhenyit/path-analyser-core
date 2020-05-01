#include "stdafx.h"
#include "FilePreProcess.h"




FilePreProcess::FilePreProcess(string & localSrc):m_sSrc(localSrc)
{
	m_sSrc = localSrc;//���ø�ֵ
	m_iMaxSize = m_sSrc.size();
	m_iOldIndex = m_iIndex = 0;
	//��ʼ���ؼ���
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
	//ֱ���ļ��������������ջ�ſ��Ե���
	//Դ�����ļ���m_sSrc��
	m_iIndex = m_sSrc.find("if");

	//ֱ��ָ���һ��if
	if (m_iIndex != m_sSrc.npos)
	{
		//�����д���if ��䣬�Ǿ�ͬʱ������else if �Ŀ�����
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
		//���ڵ�inBlockѹ����if����{����
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
					m_sSrc.insert(tempIndex, "{");//debug��ʱ����Ҫע��
					m_iIndex++;
					counter++;
					//���������else if ״̬
					NestProcess(true);


				}
				else if (m_iToken == '{')
				{
					PutBack();//������
					NestProcess(false);
					//������֮��m_iIndexӦ��ָ�����'}';
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
		//������û��if���
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
	bool flag = false;//���flag����ֻ��Ϊ������while�����е��ж�
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
			//if ���û�������ţ�֤���ǵ���䣬��Ƕ�ף�������Ҫƥ��else��
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
				//����Ƿ���else
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
            //�������else�ĵ����
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
				//else if ���εĴ���
				//putback();
				IfElsePairHelp(true);
				Match();
				//if ���û�������ţ�֤���ǵ���䣬��Ƕ�ף�������Ҫƥ��else��
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
					//����Ƿ���else
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
			//����else {}
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
//��if����е�Բ����ƥ���
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
				m_sSrc.insert(tempIndex, "{");//debug��ʱ����Ҫע��
				m_iIndex++;
				counter++;
				//���������else if ״̬
				NestProcess(true);
			}
			else if (m_iToken == '{')
			{
				PutBack();//������
				NestProcess(false);
				//������֮��m_iIndexӦ��ָ�����'}';
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

					if (!StartIndexes.empty())//��ƥ����һ��case֮ǰû������һ��break��StartIndexes���
					{
						EndIndexes.push_back(GetIndex() - 5);//��¼"case"֮ǰһ���ֽڵ��±�(GetIndex()�õ�case��һ���ֽ�' '���±꣩
					}
					Match();//match "case"
					Match();//match case value

					StartIndexes.push_back(GetIndex());
					SwitchInBlock();
					continue;
				}
				else if (GetToken() == ENUM_KEYWORD::BREAK)
				{
					if (numOfBrace != BraceNumOfSwtich)//case�����д����ţ�˵�����breakӦ����while�����break������caseƥ���break
					{
						Match();
						continue;
					}
					Match();//match ENUM_KEYWORD::BREAK
					if (StartIndexes.size() == 1)//����һ��case֮�����һ��break
					{
						StartIndexes.clear();
						continue;
					}
					else if (StartIndexes.size() == 0)//deefault֮���break
					{
						Match();
						continue;
					}
					else
					{
						EndIndexes.push_back(GetIndex());//Ӧ����Ҫ�õ���break;����;�����ֽڵ��±꣬�������ҪҪmatch ;
						for (int i = EndIndexes.size() - 2; i >= 0; i--)
						{
							//��StartIndexes[i+1]��EndIndexes[i+1]�Ĵ�����뵽EndIndexes[i]��λ��
							InsertCode(EndIndexes[i], m_sSrc.substr(StartIndexes[i + 1], EndIndexes[i + 1] - StartIndexes[i + 1]));
							//�õ��µĴ���ĩβ���±�
							int lengthOfCode = EndIndexes[i + 1] - StartIndexes[i + 1];
							EndIndexes[i] += lengthOfCode;
							for (unsigned int j = i + 1; j < StartIndexes.size(); j++)
							{
								//��Ϊ��������ˣ������index��Ҫ��Ӧλ����ô���ֽڣ�����������ɨ���ʱ����
								StartIndexes[j] += lengthOfCode;
								EndIndexes[j] += lengthOfCode;
							}
						}
						//��ʱ��*EndIndexes.2end()��¼�������һ��break����һ���ֽڵ��±꣬������Ҫ�ص�����
						//������match����Ϊm_iTokenһֱû�䣬���Ա�����һֱ�ǣ������token(�о���)
						m_iIndex = EndIndexes.back();
						StartIndexes.clear();
						EndIndexes.clear();
					}

				}
				else if (GetToken() == ENUM_KEYWORD::DEFAULT)
				{
					if (!StartIndexes.empty())//���default֮ǰ�����һ��caseû��break�����Ǹ����ӽ�ȥ
					{
						InsertCode(GetIndex() - 9, "break;");//-9��Ϊ�˻ص�default��ǰһ���ַ�
						m_iIndex += 7;//����break;��7���ַ�
						EndIndexes.push_back(GetIndex() - 9);//Ӧ����Ҫ�õ���break;����;�����ֽڵ��±꣬�������ҪҪmatch ;
						for (int i = EndIndexes.size() - 2; i >= 0; i--)
						{
							//��StartIndexes[i+1]��EndIndexes[i+1]�Ĵ�����뵽EndIndexes[i]��λ��
							InsertCode(EndIndexes[i], m_sSrc.substr(StartIndexes[i + 1], EndIndexes[i + 1] - StartIndexes[i + 1]));
							//�õ��µĴ���ĩβ���±�
							int lengthOfCode = EndIndexes[i + 1] - StartIndexes[i + 1];
							EndIndexes[i] += lengthOfCode;
							for (unsigned int j = i + 1; j < StartIndexes.size(); j++)
							{
								//��Ϊ��������ˣ������index��Ҫ��Ӧλ����ô���ֽڣ�����������ɨ���ʱ����
								StartIndexes[j] += lengthOfCode;
								EndIndexes[j] += lengthOfCode;
							}
						}
						//��ʱ��*EndIndexes.2end()��¼�������һ��break����һ���ֽڵ��±꣬������Ҫ�ص�����
						//������match����Ϊm_iTokenһֱû�䣬���Ա�����һֱ�ǣ������token(�о���)
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
						if (!StartIndexes.empty())//�����֮ǰ��û������default�������һ��caseû��break�����Ǹ����ӽ�ȥ
						{
							InsertCode(GetIndex() - 2, "break;");
							cout << m_sSrc;
							m_iIndex += 7;//����break;��7���ַ�
							EndIndexes.push_back(GetIndex() - 2);
							for (int i = EndIndexes.size() - 2; i >= 0; i--)
							{
								//��StartIndexes[i+1]��EndIndexes[i+1]�Ĵ�����뵽EndIndexes[i]��λ��
								std::cout << m_sSrc;
								InsertCode(EndIndexes[i], m_sSrc.substr(StartIndexes[i + 1], EndIndexes[i + 1] - StartIndexes[i + 1]));
								std::cout << std::endl;
								std::cout << m_sSrc;
								//�õ��µĴ���ĩβ���±�
								int lengthOfCode = EndIndexes[i + 1] - StartIndexes[i + 1];
								EndIndexes[i] += lengthOfCode;
								for (unsigned int j = i + 1; j < StartIndexes.size(); j++)
								{
									//��Ϊ��������ˣ������index��Ҫ��Ӧλ����ô���ֽڣ�����������ɨ���ʱ����
									StartIndexes[j] += lengthOfCode;
									EndIndexes[j] += lengthOfCode;
								}
							}
							//��ʱ��*EndIndexes.2end()��¼�������һ��break����һ���ֽڵ��±꣬������Ҫ�ص�����
							//������match����Ϊm_iTokenһֱû�䣬���Ա�����һֱ�ǣ������token(�о���)
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

					if (!StartIndexes.empty())//��ƥ����һ��case֮ǰû������һ��break��StartIndexes���
					{
						EndIndexes.push_back(GetIndex() - 5);//��¼"case"֮ǰһ���ֽڵ��±�(GetIndex()�õ�case��һ���ֽ�' '���±꣩
					}
					Match();//match "case"
					Match();//match case value

					StartIndexes.push_back(GetIndex());
					SwitchInBlock();						  
					continue;
				}
				else if (GetToken() == ENUM_KEYWORD::BREAK)
				{
					if (numOfBrace != BraceNumOfSwtich)//case�����д����ţ�˵�����breakӦ����while�����break������caseƥ���break
					{
						Match();
						continue;
					}
					Match();//match ENUM_KEYWORD::BREAK
					if (StartIndexes.size() == 1)//����һ��case֮�����һ��break
					{
						StartIndexes.clear();
						continue;
					}
					else if (StartIndexes.size() == 0)//deefault֮���break
					{
						Match();
						continue;
					}
					else
					{
						EndIndexes.push_back(GetIndex());//Ӧ����Ҫ�õ���break;����;�����ֽڵ��±꣬�������ҪҪmatch ;
						for (int i = EndIndexes.size() - 2; i >= 0; i--)
						{
							//��StartIndexes[i+1]��EndIndexes[i+1]�Ĵ�����뵽EndIndexes[i]��λ��
							InsertCode(EndIndexes[i], m_sSrc.substr(StartIndexes[i + 1], EndIndexes[i + 1] - StartIndexes[i + 1]));
							//�õ��µĴ���ĩβ���±�
							int lengthOfCode = EndIndexes[i + 1] - StartIndexes[i + 1];
							EndIndexes[i] += lengthOfCode;
							for (unsigned int j = i + 1; j < StartIndexes.size(); j++)
							{
								//��Ϊ��������ˣ������index��Ҫ��Ӧλ����ô���ֽڣ�����������ɨ���ʱ����
								StartIndexes[j] += lengthOfCode;
								EndIndexes[j] += lengthOfCode;
							}
						}
						//��ʱ��*EndIndexes.2end()��¼�������һ��break����һ���ֽڵ��±꣬������Ҫ�ص�����
						//������match����Ϊm_iTokenһֱû�䣬���Ա�����һֱ�ǣ������token(�о���)
						m_iIndex = EndIndexes.back();
						StartIndexes.clear();
						EndIndexes.clear();
					}

				}
				else if (GetToken() == ENUM_KEYWORD::DEFAULT)
				{
					if (!StartIndexes.empty())//���default֮ǰ�����һ��caseû��break�����Ǹ����ӽ�ȥ
					{
						InsertCode(GetIndex() - 9, "break;");//-9��Ϊ�˻ص�default��ǰһ���ַ�
						m_iIndex += 7;//����break;��7���ַ�
						EndIndexes.push_back(GetIndex() - 9);//Ӧ����Ҫ�õ���break;����;�����ֽڵ��±꣬�������ҪҪmatch ;
						for (int i = EndIndexes.size() - 2; i >= 0; i--)
						{
							//��StartIndexes[i+1]��EndIndexes[i+1]�Ĵ�����뵽EndIndexes[i]��λ��
							InsertCode(EndIndexes[i], m_sSrc.substr(StartIndexes[i + 1], EndIndexes[i + 1] - StartIndexes[i + 1]));
							//�õ��µĴ���ĩβ���±�
							int lengthOfCode = EndIndexes[i + 1] - StartIndexes[i + 1];
							EndIndexes[i] += lengthOfCode;
							for (unsigned int j = i + 1; j < StartIndexes.size(); j++)
							{
								//��Ϊ��������ˣ������index��Ҫ��Ӧλ����ô���ֽڣ�����������ɨ���ʱ����
								StartIndexes[j] += lengthOfCode;
								EndIndexes[j] += lengthOfCode;
							}
						}
						//��ʱ��*EndIndexes.2end()��¼�������һ��break����һ���ֽڵ��±꣬������Ҫ�ص�����
						//������match����Ϊm_iTokenһֱû�䣬���Ա�����һֱ�ǣ������token(�о���)
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
						if (!StartIndexes.empty())//�����֮ǰ��û������default�������һ��caseû��break�����Ǹ����ӽ�ȥ
						{
							InsertCode(GetIndex() - 2, "break;");
							cout << m_sSrc;
							m_iIndex += 7;//����break;��7���ַ�
							EndIndexes.push_back(GetIndex() - 2);
							for (int i = EndIndexes.size() - 2; i >= 0; i--)
							{
								//��StartIndexes[i+1]��EndIndexes[i+1]�Ĵ�����뵽EndIndexes[i]��λ��
								std::cout << m_sSrc;
								InsertCode(EndIndexes[i], m_sSrc.substr(StartIndexes[i + 1], EndIndexes[i + 1] - StartIndexes[i + 1]));
								std::cout << std::endl;
								std::cout << m_sSrc;
								//�õ��µĴ���ĩβ���±�
								int lengthOfCode = EndIndexes[i + 1] - StartIndexes[i + 1];
								EndIndexes[i] += lengthOfCode;
								for (unsigned int j = i + 1; j < StartIndexes.size(); j++)
								{
									//��Ϊ��������ˣ������index��Ҫ��Ӧλ����ô���ֽڣ�����������ɨ���ʱ����
									StartIndexes[j] += lengthOfCode;
									EndIndexes[j] += lengthOfCode;
								}
							}
							//��ʱ��*EndIndexes.2end()��¼�������һ��break����һ���ֽڵ��±꣬������Ҫ�ص�����
							//������match����Ϊm_iTokenһֱû�䣬���Ա�����һֱ�ǣ������token(�о���)
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
            //һ��doƥ��һ��while
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
			numOfBrace = 0;//�����ʼ��Ϊ0����ΪҪƥ����һ��;������û��ƥ�䵽;�ͳ���һ����
			//����������if(){}
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
	m_iOldIndex = m_iIndex;//ά������index ���Ի��˹���
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
			//��ѯһ�¸��ַ����Ƿ��ǹؼ���
			std::map<std::string, int>::iterator it;
			it = m_structKeyWordTable.find(name);
			if (it != m_structKeyWordTable.end())
			{
				//�ؼ���
				//m_sCurrentTokenStringValue = name;
				m_iOldToken = m_iToken;
				m_iToken = it->second;
				//�����ַ�����ö��ֵ
				return m_iToken;
			}
			//��ǰ��������ͨ����
			//m_sCurrentTokenStringValue = name;
            m_iOldToken = m_iToken;
			m_iToken = ENUM_KEYWORD::ID;
			return m_iToken;
			//���������ֵ�����Ͷ������ھ�������������һ������ģ�����涨��
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
				//��������
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
					m_iIndex++;//����ע��
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
	m_iToken = ENUM_KEYWORD::FILEEOF;//�ļ���������
	return m_iToken;
}

void FilePreProcess::PutBack()
{
	m_iIndex = m_iOldIndex;
	m_iToken = m_iOldToken;
}

void FilePreProcess::Program()
{
	//��while��for��Ԥ����һ��Ҫ����ifelseǰ��
	WhileAndForProgram();
	BackToFileBegin();
	//��ifelse��Ԥ����
	IfElsePairProgram();
	BackToFileBegin();
	InsertChar();
	BackToFileBegin();
	cout << "\n����Ԥ�������Ĵ��룺\n" << m_sSrc;
	//��switch��Ԥ����
	SwitchProgram();
	BackToFileBegin();
	
	cout << "\n����Ԥ�������Ĵ��룺\n"<<m_sSrc;
	return;
}
