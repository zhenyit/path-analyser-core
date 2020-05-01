//算法源码，需要直接插入源文件中
//DEAlgorithm2017-01-16
#include"stdafx.h"
#include<iostream>
#include<ctime>
#include<fstream>
#include<cstdlib>
using std::cout;
using std::cin;
using std::endl;
using std::fstream;

//节点表
//路径表
const int NODENUM = 4;//判断节点数
const int PATHNUM = 4;//路径数
const int R = 3;		//测试用例中，也即函数有几个参数,由扫描结果确定
const int RUN = 30;				//算法执行运行30次
const double F = 0.5;			//
const double alpha = 0.001;		//计算fitness时需要
const int K = 10;				//计算fitness的参数
const int POPNUM = 50;			//种群个数
const int MCN = 100000;			//	最大测试用例迭代次数
const double PC = 0.2;				//变异的概率
int cycle[RUN] = { 0 };			//每一次算法的运行，总共需要多少次种群迭代次数
double coverage[RUN] = { 0 };	//每一次算法运行时的路径覆盖率
double runTime[RUN] = { 0 };	//每一次算法的运行，消耗的总时间
int caseNum[RUN] = { 0 };


void DEAlgorithm()
{
	srand((unsigned)time(NULL));
	fstream output("result.txt");
	int lb[R];			//测试用例参数对应的下界，这一部分倒时时需要用户手动输入的，如果不输入,我们也需要给他补一个
	int ub[R];			//测试用例参数对应的上界
	for (int i = 0; i < R; i++)
	{
		lb[i] = 0;
		ub[i] = 1000;
	}
	time_t start;//algorithm start time;
	time_t finish;//algorithm over time;
	for (int run = 0; run < RUN; run++)
	{
		output << endl;
		cout << "Times: " << run << endl;
		output << "Times: " << run << " start:" << endl;


		int x[POPNUM][R] = { 0 };				//记录对应的测试用例
		int v[POPNUM][R] = { 0 };				//记录对应的测试用例
		double fitness_x[POPNUM];		//x测试用例对应的fitness值
		double fitness_v[POPNUM];		//v测试用例对应的fitness值
		int statu[PATHNUM];			//路径标记
		for (int i = 0; i < POPNUM; i++)
		{
			fitness_v[i] = fitness_x[i] = -1;
		}
		for (int i = 0; i < PATHNUM; i++)
		{
			statu[i] = -1;
		}

		double solution[PATHNUM][R];			//记录哪一组覆盖的对应路径

		int currentPathNum = 0;					//记录当前有几条路径了
		start = clock();	//记录当前时间
		//初始化种群
		for (int i = 0; i < POPNUM; i++)
		{
			for (int j = 0; j < R; j++)
			{
				x[i][j] = (rand() % (ub[j] - lb[j] + 1)) + lb[j];//随机生成一个测试用例
			}
			caseNum[run]++;

			//调用测试函数更新nodeTable与pathTable

			
			//计算path路径代码开始

			//计算path路径结束

			//计算当前path是否已经被覆盖过，若覆盖过直接跳过，否则进入记录，currentPathNum++;


			if (currentPathNum == PATHNUM)
			{
				break;
				//当前已经完全覆盖所有路径
			}



		}

		cycle[run] = 1;
		//若路径未被完全覆盖或者还未超过最大的测试用例迭代次数，继续迭代
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
				//DE算法的交叉变异
				int jrand = rand() % R;
				for (int j = 0; j < R; j++)
				{
					v[i][j] = (int)round(abs(x[i][j] + F * (x[k1][j] - x[k2][j])));//为什么要强制转型 F= 0.5；
					if (rand() / (double)RAND_MAX <= PC && j != jrand)
					{
						v[i][j] = x[i][j];
					}
					if (v[i][j] > ub[j] || v[i][j] < lb[j])
					{
						v[i][j] = lb[j] + rand() % (ub[j] - lb[j] + 1);
					}
				}
				//交叉变异之后产生新的测试用例
				caseNum[run] = caseNum[run] + 1;
				

				//调用测试函数部分，更新nodeTable与pathTable；



				//计算path路径代码开始

				//计算path路径结束

				//计算当前path是否已经被覆盖过，若覆盖过直接跳过，否则进入记录，currentPathNum++;
				if (currentPathNum == PATHNUM)
				{
					break;
					//当前已经完全覆盖所有路径
				}

			}

			//fitness应用场合
			for (int i = 0; i < POPNUM; i++)
			{
				if (currentPathNum == PATHNUM)
				{
					cycle[run]++;
					break;
				}
				//fitness_x[i] = calculateFitness(unorder_map* hashTable,isLeftCover);
				//fitness_v[i] = calculateFitness(v[i], unorder_map* hashTable, isRightCover);
				if (fitness_v[i] > fitness_x[i])		   //step 6：比较更新测试用例
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
		output << "运行时间= " << runTime[run] << "ms" << endl;
		output << "NO. of cycles= " << (cycle[run] - 1) << endl;
		coverage[run] = currentPathNum * 100 / PATHNUM;//覆盖率
		output << "路径覆盖率= " << coverage[run] << "%" << endl;
		output << "最优解为：" << endl;
		for (int k = 0; k < PATHNUM; k++)     //输出结果
		{
			if (statu[k] >= 0)
			{
				output << "path " << k << ": ";
				for (int j = 0; j < R; j++)
					output << solution[k][j] << " ";
				output << endl;
			}
			else
				output << "path:" << k << "没被覆盖." << endl;

		}
		output << endl << endl;
	}
}

