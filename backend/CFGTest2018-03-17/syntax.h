#pragma once
#include<vector>
#include<algorithm>
//#include<unordered_map>
#include<stack>
#include<list>
#include<memory>
#include<typeinfo>
#include<iterator>
#include"lexical.h"
namespace nsFitness
{
	extern const double K;
	extern const double EPSILON;

	//抽象语法树的数据结构
	//class ACVertex
	//{
	//public:
	//	virtual double cost()const = 0;
	//};

	//class CBooleanVertex :public ACVertex
	//{
	//private:
	//	bool b;
	//public:
	//	CBooleanVertex(bool B);
	//	double cost()const;
	//};

	//class ACBoolOpVertex :public ACVertex
	//{
	//protected:
	//	const ACVertex*left, *right;
	//public:
	//	ACBoolOpVertex(const ACVertex*Left, const ACVertex*Right);
	//	//virtual double cost()const = 0;
	//	~ACBoolOpVertex()
	//	{
	//		delete left, right;
	//	}
	//};
	//class CAndVertex :public ACBoolOpVertex
	//{
	//public:
	//	double cost()const;
	//};
	//class COrVertex :public ACBoolOpVertex
	//{
	//public:
	//	double cost()const;
	//};

	//class CNotVertex :public ACVertex
	//{
	//private:
	//	const ACVertex*boolExpr;
	//public:
	//	CNotVertex(const ACVertex*BoolExpr);
	//	double cost()const;
	//};

	//template<typename T1, typename T2>class ACRelaOpVertex :public ACVertex
	//{
	//protected:
	//	T1 a;
	//	T2 b;
	//public:
	//	ACRelaOpVertex(T1 A, T2 B)
	//	{
	//		a = A;
	//		b = B;
	//	}
	//	//virtual double cost()const = 0;
	//};
	//template<typename T1, typename T2>class CMoreVertex :public ACRelaOpVertex<T1, T2>
	//{
	//public:
	//	double cost()const
	//	{
	//		return a > b ? 0 : (b - a + K);
	//	}
	//};
	//template<typename T1, typename T2>class CMoreEqualVertex :public ACRelaOpVertex<T1, T2>
	//{
	//public:
	//	double cost()const
	//	{
	//		return a >= b ? 0 : (b - a + K);
	//	}
	//};
	//template<typename T1, typename T2>class CLessVertex :public ACRelaOpVertex<T1, T2>
	//{
	//public:
	//	double cost()const
	//	{
	//		return a < b ? 0 : (a - b + K);
	//	}
	//};
	//template<typename T1, typename T2>class CLessEqualVertex :public ACRelaOpVertex<T1, T2>
	//{
	//public:
	//	double cost()const
	//	{
	//		return a <= b ? 0 : (a - b + K);
	//	}
	//};
	//template<typename T1, typename T2>class CEqualVertex :public ACRelaOpVertex<T1, T2>
	//{
	//public:
	//	double cost()const
	//	{
	//		return a == b ? 0 : (std::abs(a - b) + K);
	//	}
	//};
	//template<typename T1, typename T2>class CNotEqualVertex :public ACRelaOpVertex<T1, T2>
	//{
	//public:
	//	double cost()const
	//	{
	//		return a != b ? 0 : K;
	//	}
	//};

	/*struct SExpr
	{
		std::string name, type;
	};*/
	//struct state
	//{
	//	size_t front;//[0,n)
	//	
	//};
	class CASTNode
	{
	public:
		SToken::tokenType tt;
		std::string str;
		virtual bool NoChildren()const;
	};
	class CDoubChildNode :public CASTNode
	{
	public:
		std::shared_ptr<CASTNode>left, right;
		bool NoChildren()const;
	};
	class CSingleChildNode :public CASTNode
	{
	public:
		std::shared_ptr<CASTNode>child;
		bool NoChildren() const;
	};
	//template<typename T>/*std::list<T>::iterator*/T next(/*std::list<T>::iterator*/T it)
	//{
	//	return ++it;      
	//}
	//template<typename T>/*std::list<T>::iterator*/T prev(/*std::list<T>::iterator*/T it)
	//{
	//	return --it;
	//}

	std::shared_ptr<CASTNode>syntaxAnalysis(const std::vector<SToken>&VToken);
	std::shared_ptr<CASTNode>buildAST(const std::vector<SToken>&v);
	std::shared_ptr<CASTNode>buildAST(std::list<std::shared_ptr<CASTNode>>&lst);
}
