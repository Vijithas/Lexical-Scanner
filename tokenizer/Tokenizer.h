#ifndef TOKENIZER_H
#define TOKENIZER_H
///////////////////////////////////////////////////////////////////////
// Tokenizer.h - read words from a std::stream                       //
// ver 3.4                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Vijitha S, Syracuse University                     //
//              vsathyan@syr.edu                                     //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a public Toker class and private ConsumeState class.
 * Toker reads words from a std::stream, throws away whitespace and comments
 * and returns words from the stream in the order encountered.  Quoted
 * strings and certain punctuators and newlines are returned as single tokens.
 *
 * This is a new version, based on the State Design Pattern.  Older versions
 * exist, based on an informal state machine design.
 *
 * Build Process:
 * --------------
 * Required Files: Tokenizer.h, Tokenizer.cpp
 * Build Command: devenv Tokenizer.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 3.4 : 09 Feb 2016
 * - added 2 function calls to set Single and Special Char Pairs
 * ver 3.3 : 02 Feb 2016
 * - declared ConsumeState copy constructor and assignment operator = delete
 * ver 3.2 : 28 Jan 2016
 * - fixed bug in ConsumeState::nextState() by returning a valid state
 *   pointer if all tests fail due to reaching end of file instead of
 *   throwing logic_error exception.
 * ver 3.1 : 27 Jan 2016
 * - fixed bug in EatCComment::eatChars()
 * - removed redundant statements assigning _pState in derived eatChars()
 *   functions
 * - removed calls to nextState() in each derived eatChars() and fixed
 *   call to nextState() in ConsumeState::consumeChars()
 * ver 3.0 : 11 Jun 2014
 * - first release of new design
 *
 * Planned Additions and Changes:
 * ------------------------------
 * - Return quoted strings as single token.  This must handle \" and \'
 *   correctly.
 * - Consider converting eatNewLine() to eatSpecialTokens() - see below
 * - Return [, ], {, }, (, ), <, >, :, ; as single character tokens
 * - Return <<, >>, +=, -=, *=, /=, :: as two character tokens
 */

#include <iosfwd>
#include <string>
#include<fstream>

namespace Scanner
{
	class ConsumeState;

	class Toker
	{
	public:
		Toker();
		Toker(const Toker&) = delete;
		~Toker();
		Toker& operator=(const Toker&) = delete;
		bool attach(std::istream* pIn);
		std::string getTok();
		bool canRead();
		void setSpecialSingleChars(std::string ssc);
		void setSpecialCharPairs(std::string scp);
		void resetConstructor();
	private:
		ConsumeState* pConsumer;

	};
}
#endif
