#pragma once
#include"testing.h"
#include<vector>
#include<string>
namespace nsFitness
{
	//词法分析的数据结构
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
