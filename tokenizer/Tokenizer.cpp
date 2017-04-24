/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// ver 3.3                                                         //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Vijitha S, CS, Syracuse University				   //
//              vsathyan@syr.edu                                   //
// Source:      Jim Fawcett, Syracuse University, CST 4-187        //
//              jfawcett@twcny.rr.com                              //
/////////////////////////////////////////////////////////////////////
/*
*
* Maintenance History :
*--------------------
* ver 3.3 : 09 Feb 2016
* -added functionalities to handle tokenizing special cases like Punctuators, Quoted strings.
* -added functionalities to handle tokenizing special cases like Single Character Pairs, Special Single Character
* -added functionalities to handle escape characters like \n,\t.
*/
#include "Tokenizer.h"
#include <iostream>
#include <sstream>
#include <cctype>
#include <string>
#include <vector>
#include <map>

namespace Scanner
{
	class ConsumeState
	{
	public:
		ConsumeState();
		virtual ~ConsumeState();
		void attach(std::istream* pIn) { _pIn = pIn; }
		virtual void eatChars() = 0;
		void consumeChars() {
			_pState->eatChars();
			_pState = nextState();
		}
		bool canRead() { return _pIn->good(); }
		std::string getTok() { return token; }
		bool hasTok() { return token.size() > 0; }
		ConsumeState* nextState();
		void setSpecialSingleChars(std::string ssc);
		void setSpecialPairs(std::string scp);
		void reset();
	protected:
		static std::string token;
		static std::istream* _pIn;
		static int prevChar;
		static int currChar;
		static ConsumeState* _pState;
		static ConsumeState* _pEatCppComment;
		static ConsumeState* _pEatCComment;
		static ConsumeState* _pEatWhitespace;
		static ConsumeState* _pEatPunctuator;
		static ConsumeState* _pEatAlphanum;
		static ConsumeState* _pEatNewline;
		//new code
		static ConsumeState* _pEatSSC;
		static ConsumeState* _pEatSCP;
		static ConsumeState* _pEatString;

		static std::map<std::string, char> SingleSpecialCharacter;
		static std::map<std::string, char> SpecialCharPairs;
		static bool enableConstructor;
		static bool enableDestructor;
	};
}

using namespace Scanner;
std::string ConsumeState::token;
std::istream* ConsumeState::_pIn;
int ConsumeState::prevChar;
int ConsumeState::currChar;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
//new code
ConsumeState* ConsumeState::_pEatSSC = nullptr;
ConsumeState* ConsumeState::_pEatSCP = nullptr;
ConsumeState* ConsumeState::_pEatString = nullptr;
//new code
ConsumeState* ConsumeState::_pEatNewline;
bool ConsumeState::enableConstructor = true;
bool ConsumeState::enableDestructor = true;

void testLog(const std::string& msg);

std::map<std::string, char> ConsumeState::SpecialCharPairs = { { "<<" ,' ' }, { ">>" ,' ' }, { ">>" ,' ' },
	{ "++" , ' ' }, { "--" ,' ' }, { "==" ,' ' }, { "+=" ,' ' }, { "-=" ,' ' }, { "*=" ,' ' }, { "/=" ,' ' } };

std::map<std::string, char> ConsumeState::SingleSpecialCharacter = { { "<", ' '},{ ">", ' ' },{ "[", ' ' },{ "]",' ' },
{ "(",' ' },{ ")",' ' },{ "{",' ' },{ "}",' ' },{ ":",' ' },{ "=",' ' },{ "+",' ' },{ "-",' ' },{ "*",' ' },{ "\n",' ' } };

ConsumeState* ConsumeState::nextState()
{
	int chNext = _pIn->peek();
	if (std::isspace(currChar) && currChar != '\n')
	{
		testLog("state: eatWhitespace");
		return _pEatWhitespace;
	}
	if (currChar == '/' && chNext == '/')
	{
		testLog("state: eatCppComment");
		return _pEatCppComment;
	}
	if (currChar == '/' && chNext == '*')
	{
		testLog("state: eatCComment");
		return _pEatCComment;
	}
	if (currChar == '*' && chNext == '/')
	{
		testLog("state: eatCComment");
		return _pEatCComment;
	}
	if (currChar == '\n')
	{
		testLog("state: eatNewLine");
		return _pEatNewline;
	}
	if (std::isalnum(currChar))
	{
		testLog("state: eatAlphanum");
		return _pEatAlphanum;
	}

	//my code
	if (ispunct(currChar) && ispunct(chNext))
	{
		testLog("state: eatTwoCharToken");
		std::vector<std::string>::iterator iter;
		char cur = currChar;
		char next = chNext;
		std::string strcurr;
		std::string strnext;
		std::stringstream ss1;
		std::stringstream ss2;
		ss1 << cur;
		ss1 >> strcurr;
		ss2 << next;
		ss2 >> strnext;
		strcurr.append(strnext);
		std::map<std::string, char>::iterator it;
		it = SpecialCharPairs.find(strcurr);
		if (it != SpecialCharPairs.end())
			return _pEatSCP;

	}
	if (ispunct(currChar))
	{
		testLog("state: eatSingleCharToken");
		std::map<std::string, char>::iterator it;
		char cur = currChar;
		std::string strcurr;

		std::stringstream ss;
		ss << cur;
		ss >> strcurr;

		it = SingleSpecialCharacter.find(strcurr);
		if (it != SingleSpecialCharacter.end())
			return _pEatSSC;
	}

	if (ispunct(currChar) && currChar != '\"')
	{
		testLog("state: eatPunctuator");
		return _pEatPunctuator;
	}

	if (currChar == '\"')
	{
		testLog("state: eatString");
		return _pEatString;
	}
	//new code
	if (!_pIn->good())
		return _pEatWhitespace;
	throw(std::logic_error("invalid type"));
}

class EatWhitespace : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		do {
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (std::isspace(currChar) && currChar != '\n');
	}
};

class EatCppComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		std::cout << "\n";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '\n');
	}
};

class EatCComment : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		std::cout << "\n ";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '*' || _pIn->peek() != '/');
		token += currChar;
		token += _pIn->peek();
		_pIn->get();
		currChar = _pIn->get();
	}
};

class EatPunctuator : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		std::cout << "\n ";
		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();

	}
};

class EatAlphanum : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		std::cout << "\n";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (isalnum(currChar));
	}
};

class EatNewline : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		std::cout << "\n";
		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	}
};

//new classes
class EatSSC : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		std::cout << "\n ";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (0);
	}
};

class EatSCP : public ConsumeState
{
public:
	virtual void eatChars()
	{
		int i = 0;
		token.clear();
		std::cout << "\n";
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
			i++;
		} while (i < 2);
	}
};

class EatString : public ConsumeState
{
public:
	virtual void eatChars()
	{
		int chNext = _pIn->peek();
		token.clear();
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
		std::cout << "\n";
		do {
			while (!ispunct(currChar))
			{
				token += currChar;
				if (!_pIn->good())  // end of stream
					return;
				currChar = _pIn->get();

			}
			if (currChar == '\"')
			{
				break;
			}
			if (currChar == '\\')
			{

				if (!_pIn->good())  // end of stream
					return;
				currChar = _pIn->get();

				switch (currChar)
				{
				case 't': token += '\\';
					token += 't'; break;
				case 'n': token += '\\';
					token += 'n'; break;
				default: token += currChar;
				}

				if (!_pIn->good())  // end of stream
					return;
				currChar = _pIn->get();
			}
			else if (currChar == ';')
			{
				token += '\"';
				token += currChar;
				token += '\"';
				if (!_pIn->good())  // end of stream
					return;
				currChar = _pIn->get();

			}
			else
			{
				token += currChar;
				if (!_pIn->good())  // end of stream
					return;
				currChar = _pIn->get();
			}

		} while (currChar != '\"');
		if (!_pIn->good())  // end of stream  
			return;
		currChar = _pIn->get();
	}
};

void ConsumeState::reset() {
	ConsumeState::enableConstructor = true;
	ConsumeState::enableDestructor = true;
}

//end new classes

ConsumeState::ConsumeState()
{
	if (ConsumeState::enableConstructor)
	{
		ConsumeState::enableConstructor = false;
		_pEatAlphanum = new EatAlphanum();
		_pEatCComment = new EatCComment();
		_pEatCppComment = new EatCppComment();
		_pEatPunctuator = new EatPunctuator();
		_pEatWhitespace = new EatWhitespace();
		_pEatNewline = new EatNewline();
		//new code
		_pEatSSC = new EatSSC();
		_pEatSCP = new EatSCP();
		_pEatString = new EatString();
		//new code

		_pState = _pEatWhitespace;
	}
}

ConsumeState::~ConsumeState()
{
	if (ConsumeState::enableDestructor)
	{
		ConsumeState::enableDestructor = false;
		delete _pEatAlphanum;
		delete _pEatCComment;
		delete _pEatCppComment;
		delete _pEatPunctuator;
		delete _pEatWhitespace;
		delete _pEatNewline;
		delete _pEatSSC;
		delete _pEatSCP;
		delete _pEatString;
	}
}

void ConsumeState::setSpecialSingleChars(std::string ssp) {
	ConsumeState::SingleSpecialCharacter.insert({ ssp,' ' });
}

void ConsumeState::setSpecialPairs(std::string ssp) {
	ConsumeState::SpecialCharPairs.insert({ ssp, ' ' });

}

Toker::Toker() : pConsumer(new EatWhitespace()) {}

Toker::~Toker() { delete pConsumer; }

bool Toker::attach(std::istream* pIn)
{
	if (pIn != nullptr && pIn->good())
	{
		pConsumer->attach(pIn);
		return true;
	}
	return false;
}

void Toker::resetConstructor() {
	pConsumer->reset();
}

std::string Toker::getTok()
{
	while (true)
	{
		pConsumer->consumeChars();
		if (pConsumer->hasTok())
			break;
		if (!pConsumer->canRead())
			return "";
	}
	return pConsumer->getTok();
}

bool Toker::canRead() { return pConsumer->canRead(); }

void Toker::setSpecialSingleChars(std::string ssc)
{
	pConsumer->setSpecialSingleChars(ssc);
}

void Toker::setSpecialCharPairs(std::string scp)
{
	pConsumer->setSpecialPairs(scp);
}

void testLog(const std::string& msg)
{
#ifdef TEST_LOG
	std::cout << "\n  " << msg;
#endif
}

//----< test stub >--------------------------------------------------

#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
	std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
	//std::string fileSpec = "../Tokenizer/Tokenizer.h";
	//std::string fileSpec = "../Tokenizer/Test.txt";

	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		return 1;
	}
	Toker toker;
	toker.attach(&in);
	while (in.good())
	{
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	}
	std::cout << "\n\n";
	return 0;
}
#endif