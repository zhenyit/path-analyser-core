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
bool nodeTable[3][2];
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
const int NODENUM = 3;
const int PATHNUM = 6;
const int R =3;

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

bool func2TF(double para0)
{tempFitness=1/(((para0<0)?0:(para0+K))+EPSILON);
return (para0<0);}
bool func2FT(double para0)
{tempFitness=1/(((para0<=0)?0:(para0+K))+EPSILON);
return !((para0<=0));}
bool func2TT(double para0)
{tempFitness=0;
return (para0<0);}
bool func2FF(double para0)
{tempFitness=1/EPSILON;
return (para0<0);}

int test_function(int a, int b, int c) {path += "0";

    
    if(
nodeTable[0][0]?(nodeTable[0][1]?func0TT((0)-(a)):func0FT((a)-(0))):(nodeTable[0][1]?func0TF((0)-(a)):func0FF((0)-(a)))) {	globalFitness += tempFitness;tempFitness = 0; 
nodeTable[0][0] = true;
path += "-1";

    }
    else {
	globalFitness += tempFitness;tempFitness = 0; 
nodeTable[0][1] = true;
path += "-2";

    }path += "-3";

    if(
nodeTable[1][0]?(nodeTable[1][1]?func1TT((0)-(b)):func1FT((b)-(0))):(nodeTable[1][1]?func1TF((0)-(b)):func1FF((0)-(b)))) {	globalFitness += tempFitness;tempFitness = 0; 
nodeTable[1][0] = true;
path += "-4";

        if(
nodeTable[2][0]?(nodeTable[2][1]?func2TT((0)-(c)):func2FT((c)-(0))):(nodeTable[2][1]?func2TF((0)-(c)):func2FF((0)-(c)))) {	globalFitness += tempFitness;tempFitness = 0; 
nodeTable[2][0] = true;
path += "-5";

        }
        else {
	globalFitness += tempFitness;tempFitness = 0; 
nodeTable[2][1] = true;
path += "-6";

        }path += "-7";

    }
    else {
	globalFitness += tempFitness;tempFitness = 0; 
nodeTable[1][1] = true;
path += "-8";

    }path += "-9";

}


int main(int argc, char* argv[]){ DEAlgorithm();}
