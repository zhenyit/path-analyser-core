#include "stdafx.h"
#include <iostream>

#include <fstream>

#include <ctime>

#include <cstdlib>

#include <vector>

#include <string>

using std::string;

using std::cout;

using std::cin;

using std::endl;

using std::vector;

using std::fstream;

void DEAlgorithm();
bool nodeTable[3][2];
std::vector<string> pathTable;
std::string path = "";
double globalFitness = 0;
double tempFitness = 0;


//这个算法代码需要
const int NODENUM = 3;
const int PATHNUM = 6;
const int R = 3;
const int K = 10;
const double EPSILON = 0.01;
const int POPNUM = 50;
const int MCN = 100000;
const int PC = 0.2;
const int F = 0.5;
const int RUN = 30;
int cycle[RUN] = { 0 };
double coverage[RUN] = { 0 };
double runTime[RUN] = { 0 };
int caseNum[RUN] = { 0 };




bool fitnessCalculate0(double para0)
{
	tempFitness = 1 / (((para0 < 0) ? 0 : (para0 + K)) + EPSILON);
	return (para0 < 0);
}
bool fitnessCalculate1(double para0)
{
	tempFitness = 1 / (((para0 < 0) ? 0 : (para0 + K)) + EPSILON);
	return (para0 < 0);
}
bool fitnessCalculate2(double para0)
{
	tempFitness = 1 / (((para0 < 0) ? 0 : (para0 + K)) + EPSILON);
	return (para0 < 0);
}

int test_function(int a, int b, int c)
{
	path += "0";
	if (fitnessCalculate0((b + c) - (a)))
	{
		globalFitness += tempFitness;//这个在算法代码未加入
		nodeTable[0][0] = true;
		path += "1";

		if (fitnessCalculate1((c)-(b)))
		{
			globalFitness += tempFitness;//这个在算法代码未加入
			nodeTable[1][0] = true;
			path += "2";

			a = 100;
		}
		path += "3";
		globalFitness += tempFitness;//这个在算法代码未加入
		a = 1;
		path += "4";
	}
	else
	{
		globalFitness += tempFitness;//这个在算法代码未加入
		nodeTable[1][1] = true;
		path += "5";

		a = -1;
	}
	path += "6";
	if (fitnessCalculate2((a)-(b)))
	{
		globalFitness += tempFitness;//这个在算法代码未加入
		nodeTable[2][0] = true;
		path += "7";
	}
	else
	{
		globalFitness += tempFitness;//这个在算法代码未加入
		nodeTable[2][1] = true;
		path += "8";
	}
	path += "9";
	return a;
}

int main(int argc, char *argv[]) { DEAlgorithm(); }

void DEAlgorithm()
{
	srand((unsigned)time(NULL));
	fstream output("result.txt");
	int lb[R];
	int ub[R];
	lb[0] = 0;
	ub[0] = 150;
	lb[1] = 0;
	ub[1] = 150;
	lb[2] = 0;
	ub[2] = 150;
	time_t start;
	time_t finish;
	for (int run = 0; run < RUN; run++)
	{
		output << endl;
		cout << "Times: " << run << endl;
		output << "Times: " << run << " start:" << endl;
		int x[POPNUM][R] = { 0 };
		int v[POPNUM][R] = { 0 };
		double fitness_x[POPNUM];
		double fitness_v[POPNUM];
		for (int i = 0; i < POPNUM; i++)
		{
			fitness_v[i] = fitness_x[i] = -1;
		}
		double solution[PATHNUM][R];
		int currentPathNum = 0;
		start = clock();
		for (int i = 0; i < POPNUM; i++)
		{
			for (int j = 0; j < R; j++)
			{
				x[i][j] = (rand() % (ub[j] - lb[j] + 1)) + lb[j];
			}
			caseNum[run]++;
			test_function(x[i][0], x[i][1], x[i][2]);
			vector<string>::iterator it;
			bool findIt = false;
			for (it = pathTable.begin(); it < pathTable.end(); it++)
			{
				if (*it == path)
				{
					findIt = true;
					path = "";
					break;
				}
			}
			if (!findIt)
			{
				pathTable.push_back(path);//算法代码需要更改
				path = "";
				for (int j = 0; j < R; j++)
				{
					solution[currentPathNum][j] = x[i][j];
				}
				currentPathNum++;
			}
			if (currentPathNum == PATHNUM)
			{
				break;
			}
		}
		cycle[run] = 1;
		while (caseNum[run] <= MCN && currentPathNum < PATHNUM)
		{
			for (int i = 0; i < POPNUM; i++)
			{
				int k1 = rand() % 50;
				while (k1 == i)
				{
					k1 = rand() % 50;
				}
				int k2 = rand() % 50;
				while (k2 == i || k2 == k1)
				{
					k2 = rand() % 50;
				}
				int jrand = rand() % R;
				for (int j = 0; j < R; j++)
				{
					v[i][j] = (int)round(abs(x[i][j] + F * (x[k1][j] - x[k2][j])));
					if (rand() / (double)RAND_MAX <= PC && j != jrand)
					{
						v[i][j] = x[i][j];
					}
					if (v[i][j] > ub[j] || v[i][j] < lb[j])
					{
						v[i][j] = lb[j] + rand() % (ub[j] - lb[j] + 1);
					}
				}
				caseNum[run] = caseNum[run] + 1;
				test_function(x[i][0], x[i][1], x[i][2]);
				vector<string>::iterator it;
				bool findIt = false;
				for (it = pathTable.begin(); it < pathTable.end(); it++)
				{
					if (*it == path)
					{
						findIt = true;
						path = "";
						break;
					}
				}
				if (!findIt)
				{
					pathTable.push_back(path);//算法代码需要更改
					path = "";
					for (int j = 0; j < R; j++)
					{
						solution[currentPathNum][j] = x[i][j];
					}
					currentPathNum++;
				}
				if (currentPathNum == PATHNUM)
				{
					break;
				}
			}
			for (int i = 0; i < POPNUM; i++)
			{
				if (currentPathNum == PATHNUM)
				{
					cycle[run]++;
					break;
				}
				test_function(x[i][0], x[i][1], x[i][2]);
				fitness_x[i] = globalFitness;
				test_function(v[i][0], v[i][1], v[i][2]);
				fitness_v[i] = globalFitness;
				if (fitness_v[i] > fitness_x[i])
				{
					for (int j = 0; j < R; j++)
						x[i][j] = v[i][j];
					fitness_x[i] = fitness_v[i];
				}
			}
			cycle[run]++;
		}
		finish = clock();
		runTime[run] = double(finish - start) * 1000 / CLOCKS_PER_SEC;
		output << "Run Time = " << runTime[run] << "ms" << endl;
		output << "NO. of cycles= " << (cycle[run] - 1) << endl;
		coverage[run] = currentPathNum * 100 / PATHNUM;
		output << "Path Coverage Percentage= " << coverage[run] << "%" << endl;
		output << "Best Solution" << endl;
		for (int k = 0; k < PATHNUM; k++)
		{
			if (k < currentPathNum)
			{
				output << "path " << pathTable[k] << ": ";
				for (int j = 0; j < R; j++)
					output << solution[k][j] << " ";
				output << endl;
			}
			else

			{
				output << "path:" << k << " no cover." << endl;
			}
		}
		output << endl
			<< endl;
	}
}
