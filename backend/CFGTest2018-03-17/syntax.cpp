#include "stdafx.h"
#include"syntax.h"
#ifdef TESTING
#include<iostream>
#endif
#define SYN_E throw "Syntax Error"
using namespace std;
namespace nsFitness
{
	/*CBooleanVertex::CBooleanVertex(bool B)
	{
		b = B;
	}
	double CBooleanVertex::cost()const
	{
		return b ? 0 : K;
	}

	ACBoolOpVertex::ACBoolOpVertex(const ACVertex*Left, const ACVertex*Right)
	{
		left = Left;
		right = Right;
	}
	double CAndVertex::cost()const
	{
		return left->cost() + right->cost();
	}
	double COrVertex::cost()const
	{
		return std::min(left->cost(), right->cost());
	}

	CNotVertex::CNotVertex(const ACVertex*BoolExpr)
	{
		boolExpr = BoolExpr;
	}
	double CNotVertex::cost()const
	{
		return boolExpr->cost();
	}*/

	/*double f(ACVertex*v)
	{
		return 1 / (v->cost() + EPSILON);
	}*/

	shared_ptr<CASTNode>syntaxAnalysis(const vector<SToken>&VToken)
	{
		//vector<SToken>v;
		list<shared_ptr<CASTNode>>lst;
		size_t i, l = VToken.size(), t;
		//SToken token;
		//合并算术表达式
		for (i = 0; i < l; ++i)if (VToken[i].tt == SToken::LeftBracket&&VToken[i + 2].tt == SToken::RightBracket) {
			/*if (VToken[i - 1].tt == SToken::NumExpr || VToken[i + 1].tt == SToken::NumExpr) {
				token.tt = SToken::NumExpr;
				token.str=VToken[i].str+VToken
			}*/
			shared_ptr<CASTNode>node(new CASTNode);
			//token.tt = SToken::NumExpr;
			node->tt = SToken::NumExpr;
			//token.str = VToken[i].str + VToken[i + 1].str + VToken[i + 2].str;
			node->str = VToken[i].str + VToken[i + 1].str + VToken[i + 2].str;
			t = 2;//i需要额外后移的量
			if (i + 3 < l&&VToken[i + 3].tt == SToken::NumExpr) {
				++t;
				//token.str += VToken[i + 3].str;
				node->str += VToken[i + 3].str;
			}
			if (i&&VToken[i - 1].tt == SToken::NumExpr) {
				//token.str = v.back().str + token.str;
				node->str = lst.back()->str + node->str;
				//v.pop_back();
				lst.pop_back();
			}
			//v.push_back(token);
			lst.push_back(node);
			i += t;
		}
		else {
			//v.push_back(VToken[i]);
			shared_ptr<CASTNode>node(new CASTNode);
			node->tt = VToken[i].tt;
			node->str = VToken[i].str;
			lst.push_back(node);
		}
#ifdef TESTING
		cout << endl;
		/*l = v.size();
		for (i = 0; i < l; ++i)cout << v[i].tt << ' ' << v[i].str << endl;*/
		list<shared_ptr<CASTNode>>::iterator it;
		for (it = lst.begin(); it != lst.end(); ++it)cout << (*it)->tt << '\t' << (*it)->str << endl;
#endif
		/*shared_ptr<CASTNode>y = buildAST(v);
		v.clear();*/
		return /*y*/buildAST(lst);
	}
	shared_ptr<CASTNode>buildAST(list<shared_ptr<CASTNode>>&lst)
	{
		stack<list<shared_ptr<CASTNode>>::iterator>leftBra;//储存左括号的位置
		list<shared_ptr<CASTNode>>::iterator it, it2;
		for (it = lst.begin(); it != lst.end(); ++it) {
			if ((*it)->tt == SToken::LeftBracket)leftBra.push(it);
			else if ((*it)->tt == SToken::RightBracket) {
				//易知此时leftBra.top()为左括号位置，it为右括号位置，之间无括号
				//it2 = leftBra.top();
				//处理关系运算符
				for (it2 = next(leftBra.top()); it2 != it; ++it2) {
					if ((*it2)->NoChildren() && (*it2)->tt == SToken::RelaOp) {
						shared_ptr<CDoubChildNode>token(new CDoubChildNode);
						token->tt = SToken::RelaOp;
						token->str = (*it2)->str;
						token->left = *prev(it2);
						token->right = *next(it2);
						lst.erase(prev(it2));
						it2 = lst.insert(lst.erase(lst.erase(it2)), token);
					}
				}
#ifdef TESTING
				cout << endl;
				for (it2 = lst.begin(); it2 != lst.end(); ++it2)cout << (*it2)->tt << '\t' << (*it2)->str << endl;
				system("pause");
#endif
				//处理非运算符
				for (it2 = /*next(leftBra.top())*/prev(it, 2); it2 != /*it*/leftBra.top(); --it2) {
#ifdef TESTING
					cout << (*it2)->tt << endl;
#endif
					if ((*it2)->NoChildren() && (*it2)->tt == SToken::Not) {
						shared_ptr<CSingleChildNode>token(new CSingleChildNode);
						token->tt = SToken::Not;
						token->str = (*it2)->str;
						token->child = *next(it2);
						lst.erase(next(it2));
						*it2 = token;
					}
				}
#ifdef TESTING
				cout << endl;
				for (it2 = lst.begin(); it2 != lst.end(); ++it2)cout << (*it2)->tt << '\t' << (*it2)->str << endl;
				system("pause");
#endif
				//处理且运算符
				for (it2 = next(leftBra.top()); it2 != it; ++it2)
					if ((*it2)->NoChildren() && (*it2)->tt == SToken::And) {
						shared_ptr<CDoubChildNode>token(new CDoubChildNode);
						token->tt = SToken::And;
						token->str = (*it2)->str;
						token->left = *prev(it2);
						token->right = *next(it2);
						lst.erase(prev(it2));
						lst.erase(next(it2));
						*it2 = token;
					}
				//处理或运算符
				for (it2 = next(leftBra.top()); it2 != it; ++it2)
					if ((*it2)->NoChildren() && (*it2)->tt == SToken::Or) {
						shared_ptr<CDoubChildNode>token(new CDoubChildNode);
						token->tt = SToken::Or;
						token->str = (*it2)->str;
						token->left = *prev(it2);
						token->right = *next(it2);
						lst.erase(prev(it2));
						lst.erase(next(it2));
						*it2 = token;
			}
#ifdef TESTING
				cout << endl;
				for (it2 = lst.begin(); it2 != lst.end(); ++it2)cout << (*it2)->tt << '\t' << (*it2)->str << endl;
#endif
				it2 = next(leftBra.top());
				if (it2 == prev(it)) {
					lst.erase(leftBra.top());
					leftBra.pop();
					lst.erase(it);
					it = it2;
				}
				else SYN_E;
			}
		}
		//处理括号外的
		for (it2 = lst.begin(); it2 != lst.end(); ++it2) {
			if ((*it2)->NoChildren() && (*it2)->tt == SToken::RelaOp) {
				shared_ptr<CDoubChildNode>token(new CDoubChildNode);
				token->tt = SToken::RelaOp;
				token->str = (*it2)->str;
				token->left = *prev(it2);
				token->right = *next(it2);
				lst.erase(prev(it2));
				lst.erase(next(it2));
				//it2 = lst.insert(lst.erase(lst.erase(it2)), token);
				*it2 = token;
			}
		}
#ifdef TESTING
		cout << endl;
		for (it2 = lst.begin(); it2 != lst.end(); ++it2)cout << (*it2)->tt << '\t' << (*it2)->str << endl;
#endif
		if (lst.size() >= 2) {
			for (it2 = prev(lst.end(), 2); it2 != lst.begin(); --it2) {
				if ((*it2)->NoChildren() && (*it2)->tt == SToken::Not) {
					shared_ptr<CSingleChildNode>token(new CSingleChildNode);
					token->tt = SToken::Not;
					token->str = (*it2)->str;
					token->child = *next(it2);
					lst.erase(next(it2));
					*it2 = token;
				}
			}
			if (lst.front()->NoChildren() && lst.front()->tt == SToken::Not) {
				shared_ptr<CSingleChildNode>token(new CSingleChildNode);
				token->tt = SToken::Not;
				token->str = lst.front()->str;
				token->child = *next(lst.begin());
				lst.erase(next(lst.begin()));
				lst.front() = token;
			}
		}
		for (it2 = lst.begin(); it2 != lst.end(); ++it2)
			if ((*it2)->NoChildren() && (*it2)->tt == SToken::And) {
				shared_ptr<CDoubChildNode>token(new CDoubChildNode);
				token->tt = SToken::And;
				token->str = (*it2)->str;
				token->left = *prev(it2);
				token->right = *next(it2);
				lst.erase(prev(it2));
				lst.erase(next(it2));
				*it2 = token;
			}
		for (it2 = lst.begin(); it2 != lst.end(); ++it2)
			if ((*it2)->NoChildren() && (*it2)->tt == SToken::Or) {
				shared_ptr<CDoubChildNode>token(new CDoubChildNode);
				token->tt = SToken::Or;
				token->str = (*it2)->str;
				token->left = *prev(it2);
				token->right = *next(it2);
				lst.erase(prev(it2));
				lst.erase(next(it2));
				*it2 = token;
			}
#ifdef TESTING
		cout << endl;
		for (it = lst.begin(); it != lst.end(); ++it)cout << (*it)->tt << '\t' << (*it)->str << endl;
#endif
		if (next(lst.begin()) != lst.end())SYN_E;
		return lst.front();
	}
	shared_ptr<CASTNode>buildAST(const vector<SToken>&v)
	{
		//static const string exprName("expr");
		//unordered_map<string, string>expr;
		size_t numOfExpr = 0, i, l = v.size();
		list<shared_ptr<CASTNode>>lst;
		for (i = 0; i < l; ++i) {
			shared_ptr<CASTNode>t(new CASTNode);
			t->tt = v[i].tt;
			t->str = v[i].str;
			lst.push_back(t);
		}
		return buildAST(lst);
	}
	bool CASTNode::NoChildren()const
	{
		return true;
	}
	bool CSingleChildNode::NoChildren()const
	{
		return false;
	}
	bool CDoubChildNode::NoChildren()const
	{
		return false;
	}
}
