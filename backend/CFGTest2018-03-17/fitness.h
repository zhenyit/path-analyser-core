#pragma once
#include<sstream>
#include"syntax.h"
namespace nsFitness
{
	//typedef std::unordered_map<std::string, void*> symbolTable_t;

	//double f(const ACVertex*v);
	//单个fitness，即f
	class CF
	{
	private:
		struct parameter
		{
			enum paraType 
			{
				Boolean, Double
			};
			paraType pt;//参数类型
			std::string expr/*实参*/, paraName/*形参*/;
		};
		struct bodyExpr
		{
			std::string value, cost;
		};
	public:
		static const std::string paraName0;
		static const std::string funcName0;
		static std::string nodeTableName;
	private:
		//symbolTable_t*symbolTable;
		std::string func, expr;//都带括号
		bodyExpr Makefunc(std::vector<parameter>&VPara, std::shared_ptr<CASTNode>ast, bool isTrue = true);
	public:
		//CF(symbolTable_t*SymbolTable);
		void calcuF(const std::string&expression, const std::string&fVaribleName, size_t th);
		/*const std::string&getFormalParaTF()const;
		const std::string&getActualParaTF()const;
		const std::string&getFuncBodyTF()const;
		const std::string&getFormalParaFT()const;
		const std::string&getActualParaFT()const;
		const std::string&getFuncBodyFT()const;*/
		const std::string&getFunc()const;
		const std::string&getExpr()const;
	};
	//double f(symbolTable_t*symbolTable, const std::string&expression);
}
