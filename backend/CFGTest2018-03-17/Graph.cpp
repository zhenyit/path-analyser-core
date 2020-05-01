#include "stdafx.h"
#include<iostream>
#include<fstream>
#include<sstream>
#include "Graph.h"

void Graph::DFS(int rootVertex)
{
	if (m_vectorAdjacentList[rootVertex].empty())
	{
		m_iPathNum++;
		return;
	}
	else
	{ 
		for (auto it = m_vectorAdjacentList[rootVertex].begin(); it != m_vectorAdjacentList[rootVertex].end(); it++)
		{
			DFS(*it);
		}
	}
}


std::string Graph::IntToString(int x)
{
	char buffer[20];
	_itoa_s(x, buffer, 10);
	std::string buf(buffer);
	return buf;

    std::stringstream ss;
    std::string str;
    ss << x;
    ss >> str;
    return str;

}


Graph::Graph()
{
	m_iVertexNum = 0;
}

Graph::~Graph()
{

}

int Graph::AddVertex()
{
	std::list<int> list;
	m_vectorAdjacentList.push_back(list);
	return m_iVertexNum++;
}

void Graph::AddEdge(int parent, int children)
{
	if (parent >= m_iVertexNum || children >= m_iVertexNum)
		return;
	m_vectorAdjacentList[parent].push_back(children);
}

std::vector<std::list<int> > Graph::GetAdjacentList() const
{
	return m_vectorAdjacentList;
}
//走到最底层的叶子节点

int Graph::ToGround(int v)
{
	int groundVertex = v;
	ToGround(v, groundVertex);
	return groundVertex;
}

void  Graph::ToGround(int v, int& max)
{
	if (v > max)
	{
		max = v;
	}
	if (m_vectorAdjacentList[v].empty())
	{
		return;
	}
	else
	{
		for (auto it = m_vectorAdjacentList[v].begin(); it != m_vectorAdjacentList[v].end(); it++)
		{
			if (*it > v)//如果比他小就走到了while的循环了
			{
				ToGround(*it, max);
			}
		}
			
	}
}

//找父亲节点的主要作用是？
int Graph::FindParent(int v)
{
	for (int i = 0; i < v; i++)
	{
		for (auto it = m_vectorAdjacentList[i].begin(); it != m_vectorAdjacentList[i].end(); it++)
		{
			if (*it == v)
				return i;
		}
	}
	return -1;
}

void Graph::PrintList()
{
	
	for (int i=0; i != m_vectorAdjacentList.size(); i++)
	{
		std::cout << i << ":";
		for (auto it = m_vectorAdjacentList[i].begin();it != m_vectorAdjacentList[i].end(); it++)
		{
			std::cout << *it << "  ";
		}
		std::cout << std::endl;
	}
	using std::endl;
	std::ofstream fout;
	using std::string;
	fout.open("CFG.gv");
	fout << "digraph coder {  graph[]" << std::endl;
	fout << "resolution = 600" << endl;
	fout << "node [width = 0.5,shape=circle,color=\"#2A579A\",fontcolor =\"#FFFFFF\", shape=circle, style=filled,fillcolor =\"#2A579A\"]" << endl;
	for (int i = 0; i != m_vectorAdjacentList.size(); i++)
	{
		fout << i << "->{";
		string childNode =""; 
		for (auto  it = m_vectorAdjacentList[i].begin(); it != m_vectorAdjacentList[i].end(); it++)
		{
			childNode.append(IntToString(*it));
			childNode.append(",");
		}
		if (childNode.size() == 0)
		{
			fout << "}" << endl;
		}
		else
		{
			fout << childNode.substr(0, childNode.size() - 1) << "}" << endl;
		}
	}
	fout << "}" << endl;
	fout.close();


}

int Graph::CalculatePath()
{
	DFS(0);
	return m_iPathNum;
	
}

int Graph::GetCurrentVertex() const
{
	return m_iVertexNum -1;
}
