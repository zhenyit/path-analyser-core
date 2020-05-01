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
	int AddVertex();//���һ���µĽڵ㣬���������ӽڵ�ı��
	void AddEdge(int v1, int v2);//���һ��v1->v2�ı�
	std::vector<std::list<int> > GetAdjacentList()const;//�õ��ڼ�����
	int ToGround(int v);//��v����һֱ�ߵ���v��Ŵ��Ҷ�ӽڵ�
	int FindParent(int v);//�õ�v�ĸ��ڵ�
	void PrintList();//��ӡ�ڼ�����
	int CalculatePath();//����ͼ��·����
	int GetCurrentVertex() const;//���ص�ǰ�ǽڵ���vertex - 1
private:
    std::vector<std::list<int> > m_vectorAdjacentList;
    int m_iVertexNum;
    int m_iPathNum;
    void DFS(int rootVertex);//�����������ͼ�����ڼ���·����
    void ToGround(int v, int& max);
    std::string IntToString(int x);
};

#endif // !AUTO_SOFTWARE_TEST_GRAPH_H