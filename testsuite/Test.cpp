///////////////////////////////////////////////////////////////////////
// SemiExpression.h - collect tokens for analysis                    //
// ver 3.2                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Source:      Vijitha S, CS, Syracuse University			         //
//              vsathyan@syr.edu                                     //
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
*/

#include <iostream>
#include "Test.h"
#include "../SemiExp/SemiExp.h"
#include "../tokenizer/Tokenizer.h"

using namespace Scanner;
using namespace std;

Test::Test()
{

}
Test::~Test()
{
}
int Test::testTokenizer() {
	std::string fileSpec = "testTokenizer.txt";


	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	Test t;
	Toker toker;

	toker.setSpecialCharPairs("!=");
	toker.setSpecialSingleChars(":");

	toker.attach(&in);
	while (in.good())
	{
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "-- " << tok;
	}
	std::cout << "\n\n";
	toker.resetConstructor();
	return 0;
}
int Test::testSemiExp() {

	Test t;
	Toker toker1;
	std::string fileSpec = "testSemiExp.txt";
	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return 1;
	}
	toker1.attach(&in);

	SemiExp semi(&toker1);

	while (semi.get())
	{
		std::cout << "\n  -- semiExpression --\n";
		semi.show();
		std::cout << "\n";
		if (!in.good())break;

	}
	/*
	May have collected tokens, but reached end of stream
	before finding SemiExp terminator.
	*/
	if (semi.length() > 0)
	{
		std::cout << "\n  -- semiExpression --\n";
		semi.show();
		std::cout << "\n";
	}
	toker1.resetConstructor();

	return 0;
}
int Test::testItok() {
	Test t;
	Toker toker2;
	std::string fileSpec = "iTokTest.txt";
	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return 1;
	}
	toker2.attach(&in);

	SemiExp semi(&toker2);

	cout << "=================Demonstrating Requirement 10===============================\n";

	while (semi.get())
	{
		semi.show();
		std::cout << "\n";
		if (!in.good())break;

	}
	/*
	May have collected tokens, but reached end of stream
	before finding SemiExp terminator.
	*/
	cout << "\npush_back() is implemented at line 39 in SemiExp.cpp\n";
	cout << "\n before inserting the word inserted:\n";
	semi.show();
	semi.push_back("inserted");
	cout << "\n after inserting the word inserted:\n";
	semi.show();

	int length = semi.length();
	cout << "\n length() is implemented at line 72 in SemiExp.cpp\n ";
	cout << "\n Length of the SemiExpression:" << length << "\n";

	cout << "\nremove(const std::string& tok) is defined at line 87 in SemiExp.cpp\n";
	semi.remove("inserted");
	cout << "\n after removing inserted\n";
	semi.show();

	cout << "\nremove(size_t i)is implemented at line 90 in SemiExp.cpp\n ";
	semi.remove(3);
	cout << "\n after removing 3rd element";
	semi.show();

	cout << "\nfind() is implemented at line 89 in SemiExp.cpp\n";
	cout << "\ntoLower() is defined at line 112 in SemiExp.cpp\n";
	cout << "\nmerge() is defined at line 108 in SemiExp.cpp\n";

	cout << "\nclear() is called now and is implemented at line 29 in SemiExp.cpp \n ";
	semi.clear();
	cout << "\n after clear() is called - tokens are empty - semi.show()\n";
	semi.show();

	return 0;

}
int main()
{
	Test t1;
	t1.testTokenizer();

	Test t2;
	t2.testSemiExp();

	Test t3;
	t3.testItok();
}


