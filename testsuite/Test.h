#ifndef TEST_H
#define TEST_H
///////////////////////////////////////////////////////////////////////
// Test.h -     Test package for Tokenizer and Semi Expression       //
// ver 1.0                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Vijitha S, CS, Syracuse University, CST 4-187        //
//              vsathyan@syr.edu									 //
///////////////////////////////////////////////////////////////////////

#include <iostream>
#include "../SemiExp/SemiExp.h"
#include "../tokenizer/Tokenizer.h"
//using namespace Scanner;

class Test
{
public:
	Test();
	~Test();
	int testTokenizer();
	int testSemiExp();
	int testItok();
};
#endif