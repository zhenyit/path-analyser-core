#include<iostream>
#include<fstream>
#include<ctime>
#include<cstdlib>
#include<vector>
#include<string>
#include<cmath>
#include<algorithm>
using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::fstream;

void DEAlgorithm();
bool nodeTable[2][2];
std::vector<string>pathTable;
std::string path = "";
double globalFitness = 0;
double tempFitness = 0;
const double K = 10;	
const double EPSILON = 0.01;
const int POPNUM = 50;
const int MCN = 100000;
const double PC = 0.2;
	const double F = 0.5;
const int RUN = 30;
int cycle[RUN] = { 0 };
double coverage[RUN] = { 0 };	
double runTime[RUN] = { 0 };
int caseNum[RUN] = { 0 };
const int NODENUM = 2;
const int PATHNUM = 20;
const int R =1;

bool func0TF(double para0)
{tempFitness=1/(((para0<0)?0:(para0+K))+EPSILON);
return (para0<0);}
bool func0FT(double para0)
{tempFitness=1/(((para0<=0)?0:(para0+K))+EPSILON);
return !((para0<=0));}
bool func0TT(double para0)
{tempFitness=0;
return (para0<0);}
bool func0FF(double para0)
{tempFitness=1/EPSILON;
return (para0<0);}

bool func1TF(double para0)
{tempFitness=1/(((para0<0)?0:(para0+K))+EPSILON);
return (para0<0);}
bool func1FT(double para0)
{tempFitness=1/(((para0<=0)?0:(para0+K))+EPSILON);
return !((para0<=0));}
bool func1TT(double para0)
{tempFitness=0;
return (para0<0);}
bool func1FF(double para0)
{tempFitness=1/EPSILON;
return (para0<0);}

int test_function(int a)
{path += "0";

bool flag_1 = true;
bool flag_while_condition_node_1 = false;
path += "-1";


          for(int i = 20;nodeTable[0][0]?(nodeTable[0][1]?func0TT((10)-(i)):func0FT((i)-(10))):(nodeTable[0][1]?func0TF((10)-(i)):func0FF((10)-(i))); i-=2)
          {
if (flag_1){
	globalFitness += tempFitness;tempFitness = 0; 
nodeTable[0][0] = true;
path += "-2";
}
if( flag_while_condition_node_1) {
path += "-1";
flag_while_condition_node_1= false;
}

              if(
nodeTable[1][0]?(nodeTable[1][1]?func1TT((20)-(i)):func1FT((i)-(20))):(nodeTable[1][1]?func1TF((20)-(i)):func1FF((20)-(i))))
              {if( flag_1) 
{
	globalFitness += tempFitness;tempFitness = 0; 
nodeTable[1][0] = true;
path += "-3";
}


              }
              else
              {
if ( flag_1)
{
	globalFitness += tempFitness;tempFitness = 0; 
nodeTable[1][1] = true;
path += "-4";
}


              }if ( flag_1)
{
path += "-5";
}

          if (flag_1) { flag_while_condition_node_1 = true ;}
flag_1 = false ;
}nodeTable[0][1] = true;
path +="-6";

           
    return 0;
}
int main(int argc, char* argv[]){ DEAlgorithm();}
void DEAlgorithm(){srand((unsigned)time(NULL));	fstream output("result.txt");int lb[R];		int ub[R];		lb[0]=0;ub[0]=-3.80456e-270;time_t start;time_t finish;for (int run = 0; run < RUN; run++){pathTable.clear();for (int i = 0; i < NODENUM; i++){for (int j = 0; j < 2; j++){nodeTable[i][j] = false;}}output << endl;cout << "Times: " << run << endl;output << "Times: " << run << " start:" << endl;int x[POPNUM][R] = { 0 };	int v[POPNUM][R] = { 0 };	double fitness_x[POPNUM];	double fitness_v[POPNUM];	for (int i = 0; i < POPNUM; i++){fitness_v[i] = fitness_x[i] = -1;}double solution[PATHNUM][R];int currentPathNum = 0;start = clock();	for (int i = 0; i < POPNUM; i++){for (int j = 0; j < R; j++){x[i][j] = (rand() % (ub[j] - lb[j] + 1)) + lb[j];}caseNum[run]++;test_function(x[i][0]);globalFitness = 0; vector<string>::iterator it;bool findIt = false;for (it = pathTable.begin(); it < pathTable.end(); it++){if (*it == path){ findIt = true;path = "";break;}}if (!findIt){pathTable.push_back(path);path = "";for (int j = 0; j < R; j++){solution[currentPathNum][j] = x[i][j];}currentPathNum++;}if (currentPathNum == PATHNUM){break;}}cycle[run] = 1;while (caseNum[run] <= MCN && currentPathNum < PATHNUM){for (int i = 0; i < POPNUM; i++){int k1 = rand() % 50;while (k1 == i){k1 = rand() % 50;}int k2 = rand() % 50;while (k2 == i || k2 == k1){k2 = rand() % 50;}int jrand = rand() % R;for (int j = 0; j < R; j++){v[i][j] = (int)round(abs(x[i][j] + F * (x[k1][j] - x[k2][j])));if (rand() / (double)RAND_MAX <= PC && j != jrand){v[i][j] = x[i][j];}if (v[i][j] > ub[j] || v[i][j] < lb[j]){v[i][j] = lb[j] + rand() % (ub[j] - lb[j] + 1);}}caseNum[run] = caseNum[run] + 1;test_function(v[i][0]);globalFitness = 0;globalFitness = 0; vector<string>::iterator it;bool findIt = false;for (it = pathTable.begin(); it < pathTable.end(); it++){if (*it == path){ findIt = true;path = "";break;}}if (!findIt){pathTable.push_back(path);path = "";for (int j = 0; j < R; j++){solution[currentPathNum][j] = v[i][j];}currentPathNum++;}if (currentPathNum == PATHNUM){break;}}for (int i = 0; i < POPNUM; i++){if (currentPathNum == PATHNUM){cycle[run]++;break;}test_function(x[i][0]);fitness_x[i] = globalFitness;path = "";globalFitness = 0;test_function(v[i][0]);fitness_v[i] = globalFitness;path = "";globalFitness = 0;if (fitness_v[i] > fitness_x[i]){for (int j = 0; j < R; j++)x[i][j] = v[i][j];fitness_x[i] = fitness_v[i];}}cycle[run]++;}finish = clock();runTime[run] = double(finish - start) * 1000 / CLOCKS_PER_SEC;output << "Run Time = " << runTime[run] << "ms" << endl;output << "NO. of cycles= " << (cycle[run] - 1) << endl;coverage[run] = currentPathNum * 100 / PATHNUM;output << "Path Coverage Percentage= " << coverage[run] << "%" << endl;output << "Best Solution" << endl;for (int k = 0; k < PATHNUM; k++){if (k < currentPathNum){output << "path " << pathTable[k] <<": ";for (int j = 0; j < R; j++)output << solution[k][j] << " ";output << endl;}else
{output << "path" << k << ": no cover." << endl;}}output << endl << endl;}}
