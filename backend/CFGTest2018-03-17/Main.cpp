#include "stdafx.h"
#include<iostream>
#include<vector>
#include"codeinsert.h"
#include"Lexer.h"
using namespace std;
int main(int argc, char* argv[])
{
    char** test_array;
    test_array = new char*[2];
    test_array[0] = (char*)"test";
      
    test_array[1] = (char*)"0,150,0,150,0,150";
	CodeInsert ci("test.cpp", argv);

	ci.Program();


 	return 0;
}