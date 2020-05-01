//�㷨Դ�룬��Ҫֱ�Ӳ���Դ�ļ���
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

//�ڵ��
//·����
const int NODENUM = 4;//�жϽڵ���
const int PATHNUM = 4;//·����
const int R = 3;		//���������У�Ҳ�������м�������,��ɨ����ȷ��
const int RUN = 30;				//�㷨ִ������30��
const double F = 0.5;			//
const double alpha = 0.001;		//����fitnessʱ��Ҫ
const int K = 10;				//����fitness�Ĳ���
const int POPNUM = 50;			//��Ⱥ����
const int MCN = 100000;			//	������������������
const double PC = 0.2;				//����ĸ���
int cycle[RUN] = { 0 };			//ÿһ���㷨�����У��ܹ���Ҫ���ٴ���Ⱥ��������
double coverage[RUN] = { 0 };	//ÿһ���㷨����ʱ��·��������
double runTime[RUN] = { 0 };	//ÿһ���㷨�����У����ĵ���ʱ��
int caseNum[RUN] = { 0 };


void DEAlgorithm()
{
	srand((unsigned)time(NULL));
	fstream output("result.txt");
	int lb[R];			//��������������Ӧ���½磬��һ���ֵ�ʱʱ��Ҫ�û��ֶ�����ģ����������,����Ҳ��Ҫ������һ��
	int ub[R];			//��������������Ӧ���Ͻ�
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


		int x[POPNUM][R] = { 0 };				//��¼��Ӧ�Ĳ�������
		int v[POPNUM][R] = { 0 };				//��¼��Ӧ�Ĳ�������
		double fitness_x[POPNUM];		//x����������Ӧ��fitnessֵ
		double fitness_v[POPNUM];		//v����������Ӧ��fitnessֵ
		int statu[PATHNUM];			//·�����
		for (int i = 0; i < POPNUM; i++)
		{
			fitness_v[i] = fitness_x[i] = -1;
		}
		for (int i = 0; i < PATHNUM; i++)
		{
			statu[i] = -1;
		}

		double solution[PATHNUM][R];			//��¼��һ�鸲�ǵĶ�Ӧ·��

		int currentPathNum = 0;					//��¼��ǰ�м���·����
		start = clock();	//��¼��ǰʱ��
		//��ʼ����Ⱥ
		for (int i = 0; i < POPNUM; i++)
		{
			for (int j = 0; j < R; j++)
			{
				x[i][j] = (rand() % (ub[j] - lb[j] + 1)) + lb[j];//�������һ����������
			}
			caseNum[run]++;

			//���ò��Ժ�������nodeTable��pathTable

			
			//����path·�����뿪ʼ

			//����path·������

			//���㵱ǰpath�Ƿ��Ѿ������ǹ��������ǹ�ֱ����������������¼��currentPathNum++;


			if (currentPathNum == PATHNUM)
			{
				break;
				//��ǰ�Ѿ���ȫ��������·��
			}



		}

		cycle[run] = 1;
		//��·��δ����ȫ���ǻ��߻�δ�������Ĳ�������������������������
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
				//DE�㷨�Ľ������
				int jrand = rand() % R;
				for (int j = 0; j < R; j++)
				{
					v[i][j] = (int)round(abs(x[i][j] + F * (x[k1][j] - x[k2][j])));//ΪʲôҪǿ��ת�� F= 0.5��
					if (rand() / (double)RAND_MAX <= PC && j != jrand)
					{
						v[i][j] = x[i][j];
					}
					if (v[i][j] > ub[j] || v[i][j] < lb[j])
					{
						v[i][j] = lb[j] + rand() % (ub[j] - lb[j] + 1);
					}
				}
				//�������֮������µĲ�������
				caseNum[run] = caseNum[run] + 1;
				

				//���ò��Ժ������֣�����nodeTable��pathTable��



				//����path·�����뿪ʼ

				//����path·������

				//���㵱ǰpath�Ƿ��Ѿ������ǹ��������ǹ�ֱ����������������¼��currentPathNum++;
				if (currentPathNum == PATHNUM)
				{
					break;
					//��ǰ�Ѿ���ȫ��������·��
				}

			}

			//fitnessӦ�ó���
			for (int i = 0; i < POPNUM; i++)
			{
				if (currentPathNum == PATHNUM)
				{
					cycle[run]++;
					break;
				}
				//fitness_x[i] = calculateFitness(unorder_map* hashTable,isLeftCover);
				//fitness_v[i] = calculateFitness(v[i], unorder_map* hashTable, isRightCover);
				if (fitness_v[i] > fitness_x[i])		   //step 6���Ƚϸ��²�������
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
		output << "����ʱ��= " << runTime[run] << "ms" << endl;
		output << "NO. of cycles= " << (cycle[run] - 1) << endl;
		coverage[run] = currentPathNum * 100 / PATHNUM;//������
		output << "·��������= " << coverage[run] << "%" << endl;
		output << "���Ž�Ϊ��" << endl;
		for (int k = 0; k < PATHNUM; k++)     //������
		{
			if (statu[k] >= 0)
			{
				output << "path " << k << ": ";
				for (int j = 0; j < R; j++)
					output << solution[k][j] << " ";
				output << endl;
			}
			else
				output << "path:" << k << "û������." << endl;

		}
		output << endl << endl;
	}
}

