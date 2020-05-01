#ifndef AUTO_SOFTWARE_TEST_GRAPH_H_
#define AUTO_SOFTWARE_TEST_GRAPH_H_
#include<list>
#include<string>
#include<vector>
class Graph
{

public:
	Graph();
	~Graph();
	int AddVertex();//添加一个新的节点，返回新增加节点的标号
	void AddEdge(int v1, int v2);//添加一条v1->v2的边
	std::vector<std::list<int> > GetAdjacentList()const;//得到邻间链表
	int ToGround(int v);//从v出发一直走到比v标号大的叶子节点
	int FindParent(int v);//得到v的父节点
	void PrintList();//打印邻间链表
	int CalculatePath();//计算图的路径数
	int GetCurrentVertex() const;//返回当前是节点编号vertex - 1
private:
    std::vector<std::list<int> > m_vectorAdjacentList;
    int m_iVertexNum;
    int m_iPathNum;
    void DFS(int rootVertex);//深度优先搜索图，用于计算路径数
    void ToGround(int v, int& max);
    std::string IntToString(int x);
};

#endif // !AUTO_SOFTWARE_TEST_GRAPH_H