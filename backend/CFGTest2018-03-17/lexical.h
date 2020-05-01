#pragma once
#include"testing.h"
#include<vector>
#include<string>
namespace nsFitness
{
	//�ʷ����������ݽṹ
	struct SToken
	{
		enum tokenType {
			And, RelaOp, NumExpr, LeftBracket, RightBracket, Not, Other, Or
		};
		tokenType tt;
		std::string str;
	};
	void lexicalAnalysis(std::vector<SToken>&VToken, const std::string&expression);
}
