#include "stdafx.h"
#include"fitness.h"
#define FM_E throw "FuncMaking Error"
using namespace std;
namespace nsFitness
{
	const double K = 10;
	const double EPSILON = 0.01;

	const string CF::paraName0("para");
	const string CF::funcName0("func");
	string CF::nodeTableName("nodeTable");
	/*CF::CF(symbolTable_t*SymbolTable)
	{
		symbolTable = SymbolTable;
	}*/
	CF::bodyExpr CF::Makefunc(vector<CF::parameter>&VPara/*用于储存参数列表*/, shared_ptr<CASTNode>ast/*抽象语法树*/, bool isTrue/* = true*/)
	{
		bodyExpr y;
		if (ast->tt == SToken::NumExpr) {
			stringstream ss;
			string num;
			ss << VPara.size();
			ss >> num;
			parameter p;
			p.pt = parameter::Boolean;//默认为布尔
			p.paraName = paraName0 + num;
			p.expr = isTrue ? ast->str : ("!(" + ast->str + ')');
			VPara.push_back(p);
			y.value = p.paraName;
			y.cost = '(' + p.paraName + "?0:K)";
		}
		else if (ast->tt == SToken::Not) {
			return Makefunc(VPara, ((CSingleChildNode*)ast.get())->child, !isTrue);
		}
		else if (ast->tt == SToken::And) {
			bodyExpr t1, t2;
			t1 = Makefunc(VPara, ((CDoubChildNode*)ast.get())->left, isTrue);
			t2 = Makefunc(VPara, ((CDoubChildNode*)ast.get())->right, isTrue);
			string Op;
			/*if (isTrue)Op = ast->str;
			else Op = (ast->str == "&&" ? "||" : "&&");*/
			Op = isTrue ? "&&" : "||";
			y.value = '(' + t1.value + Op + t2.value + ')';
			y.cost = (Op == "&&") ? ('(' + t1.cost + '+' + t2.cost + ')') : ("std::min(" + t1.cost + ',' + t2.cost + ')');
		}
		else if (ast->tt == SToken::Or) {
			bodyExpr t1, t2;
			t1 = Makefunc(VPara, ((CDoubChildNode*)ast.get())->left, isTrue);
			t2 = Makefunc(VPara, ((CDoubChildNode*)ast.get())->right, isTrue);
			if (isTrue) {
				y.value = '(' + t1.value + "||" + t2.value + ')';
				y.cost = "std::min(" + t1.cost + ',' + t2.cost + ')';
			}
			else {
				y.value = '(' + t1.value + "&&" + t2.value + ')';
				y.cost = '(' + t1.cost + '+' + t2.cost + ')';
			}
		}
		else if (ast->tt == SToken::RelaOp) {
			parameter p;
			stringstream ss;
			string num;
			ss << VPara.size();
			ss >> num;
			p.pt = parameter::Double;//显然关系运算符两边的操作数可以转换为double类型
			p.paraName = paraName0 + num;
			string Op;
			if (isTrue)Op = ast->str;
			else {
				if (ast->str == ">")Op = "<=";
				else if (ast->str == "<")Op = ">=";
				else if (ast->str == "==")Op = "!=";
				else if (ast->str == "!=")Op = "==";
				else if (ast->str == "<=")Op = ">";
				else if (ast->str == ">=")Op = "<";
				else FM_E;
			}
			//以下将形如A>B的式子转化为C<0, C=B-A，并将C的类型转换为double
			if (Op == ">") {
				p.expr = '(' + ((CDoubChildNode*)ast.get())->right->str + ")-(" + ((CDoubChildNode*)ast.get())->left->str + ')';
				VPara.push_back(p);
				y.value = '(' + p.paraName + "<0)";
				y.cost = '(' + y.value + "?0:(" + p.paraName + "+K))";
			}
			else if (Op == ">=") {
				p.expr = '(' + ((CDoubChildNode*)ast.get())->right->str + ")-(" + ((CDoubChildNode*)ast.get())->left->str + ')';
				VPara.push_back(p);
				y.value = '(' + p.paraName + "<=0)";
				y.cost = '(' + y.value + "?0:(" + p.paraName + "+K))";
			}
			else if (Op == "<") {
				p.expr = '(' + ((CDoubChildNode*)ast.get())->left->str + ")-(" + ((CDoubChildNode*)ast.get())->right->str + ')';
				VPara.push_back(p);
				y.value = '(' + p.paraName + "<0)";
				y.cost = '(' + y.value + "?0:(" + p.paraName + "+K))";
			}
			else if (Op == "<=") {
				p.expr = '(' + ((CDoubChildNode*)ast.get())->left->str + ")-(" + ((CDoubChildNode*)ast.get())->right->str + ')';
				VPara.push_back(p);
				y.value = '(' + p.paraName + "<=0)";
				y.cost = '(' + y.value + "?0:(" + p.paraName + "+K))";
			}
			else if (Op == "==") {
				p.expr = '(' + ((CDoubChildNode*)ast.get())->left->str + ")-(" + ((CDoubChildNode*)ast.get())->right->str + ')';
				VPara.push_back(p);
				y.value = '(' + p.paraName + "==0)";
				y.cost = '(' + y.value + "?0:(std::abs(" + p.paraName + ")+K))";
			}
			else if (Op == "!=") {
				p.expr = '(' + ((CDoubChildNode*)ast.get())->left->str + ")-(" + ((CDoubChildNode*)ast.get())->right->str + ')';
				VPara.push_back(p);
				y.value = '(' + p.paraName + "!=0)";
				y.cost = '(' + y.value + "?0:K)";
			}
			else FM_E;
		}
		return y;
	}
	void CF::calcuF(const string&expression/*表达式*/, const string&fVaribleName/*所生成函数的外部变量，用于储存本节点f的值*/, size_t th)
	{
		//using namespace std;
		size_t i, l = expression.length(), j = 0;
		char*chrT = new char[l + 1];
		for (i = 0; i < l; ++i) {//去掉字符串中的空格等
			if (!(expression[i] == ' ' || expression[i] == '\t' || expression[i] == '\n' || expression[i] == '\r'))
				chrT[j++] = expression[i];
		}
		chrT[j] = '\0';
		vector<SToken>VToken;
		lexicalAnalysis(VToken, /*expression*/chrT);//词法分析
		delete[]chrT;
		shared_ptr<CASTNode>ast = syntaxAnalysis(VToken);//语法分析，生成抽象语法树
		VToken.clear();
		std::vector<parameter>VPara;
		bodyExpr beTF = Makefunc(VPara, ast);//生成函数代码
		vector<parameter>VPara2;
		bodyExpr beFT = Makefunc(VPara2, ast, false);
		//将向量中的参数列表组合成一个参数列表字符串
		string formalParaTF, actualParaTF, funcBodyTF, formalParaFT, actualParaFT, funcBodyFT;
		l = VPara.size();
		if (l) {
			formalParaTF = (VPara.front().pt == parameter::Boolean ? "(bool " : "(double ") + VPara.front().paraName;
			actualParaTF = '(' + VPara.front().expr;
		}
		for (i = 1; i < l; ++i) {
			formalParaTF += (VPara[i].pt == parameter::Boolean ? ",bool " : ",double ") + VPara[i].paraName;
			actualParaTF += ',' + VPara[i].expr;
		}
		formalParaTF += ')';
		actualParaTF += ')';
		//生成函数体
		funcBodyTF = '{' + fVaribleName + "=1/(" + beTF.cost + "+EPSILON);\nreturn " + beTF.value + ";}";
		l = VPara2.size();
		if (l) {
			formalParaFT = (VPara2.front().pt == parameter::Boolean ? "(bool " : "(double ") + VPara2.front().paraName;
			actualParaFT = '(' + VPara2.front().expr;
		}
		for (i = 1; i < l; ++i) {
			formalParaFT += (VPara2[i].pt == parameter::Boolean ? ",bool " : ",double ") + VPara2[i].paraName;
			actualParaFT += ',' + VPara2[i].expr;
		}
		formalParaFT += ')';
		actualParaFT += ')';
		funcBodyFT = '{' + fVaribleName + "=1/(" + beFT.cost + "+EPSILON);\nreturn !(" + beFT.value + ");}";
		string funcBodyTT = '{' + fVaribleName + "=0;\nreturn " + beTF.value + ";}",
			funcBodyFF = '{' + fVaribleName + "=1/EPSILON;\nreturn " + beTF.value + ";}";
		string funcNameTF, funcNameFT, funcNameTT, funcNameFF;
		stringstream ss;
		ss << th;
		string strTh;
		ss >> strTh;
		funcNameTF = funcName0 + strTh;
		funcNameFT = funcNameTF + "FT";
		funcNameTT = funcNameTF + "TT";
		funcNameFF = funcNameTF + "FF";
		funcNameTF += "TF";
		func = "\nbool " + funcNameTF + formalParaTF + '\n' + funcBodyTF;
		func += "\nbool " + funcNameFT + formalParaFT + '\n' + funcBodyFT;
		func += "\nbool " + funcNameTT + formalParaTF + '\n' + funcBodyTT;
		func += "\nbool " + funcNameFF + formalParaTF + '\n' + funcBodyFF;
		string NTTh = nodeTableName + '[' + strTh + ']';
		string b2(NTTh + "[1]");
		expr = NTTh + "[0]?(" + b2 + '?' + funcNameTT + actualParaTF + ':' + funcNameFT + actualParaFT + "):("
			+ b2 + '?' + funcNameTF + actualParaTF + ':' + funcNameFF + actualParaTF + ')';//注意原来的TF和FT调转了
	}
	/*const string&CF::getFormalPara()const
	{
		return formalPara;
	}
	const string&CF::getActualPara()const
	{
		return actualParaTF;
	}
	const string&CF::getFuncBody()const
	{
		return funcBodyTF;
	}*/
	const string&CF::getExpr()const
	{
		return expr;
	}
	const string&CF::getFunc()const
	{
		return func;
	}
	/*double f(symbolTable_t*symbolTable, const string&expression)
	{
		return CF(symbolTable).f(expression);
	}*/
}
