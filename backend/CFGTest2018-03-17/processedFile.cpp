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
