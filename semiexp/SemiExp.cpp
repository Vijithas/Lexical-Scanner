///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                  //
// ver 3.2                                                           //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Vijitha S, CS, Syracuse University,					 //
//              vsathyan@syr.edu									 //
// Source:      Jim Fawcett, Syracuse University, CST 4-187          //
//              jfawcett@twcny.rr.com                                //
///////////////////////////////////////////////////////////////////////


#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <exception>
#include "SemiExp.h"
#include "../Tokenizer/Tokenizer.h"

using namespace Scanner;
int hash = 0;
SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}

bool SemiExp::get(bool clear)
{
	if (_pToker == nullptr)
		throw(std::logic_error("no Toker reference"));
	_tokens.clear();
	int newline = 0;
	std::string token;

	while (true) {
		std::string c = "0";
		if (newline == 0) {
			token = _pToker->getTok();
		}
		if (hash == 1) {
			_tokens.push_back("#");
			_tokens.push_back(token);
			hash = 0;
		}
		else _tokens.push_back(token);
		if (token == "{")	return true;
		if (token == "}")	return true;
		if (token == ":" && (_tokens[_tokens.size() - 2] == "public" || _tokens[_tokens.size() - 2] == "private" || _tokens[_tokens.size() - 2] == "protected"))
			return true;
		if (token == ";" && !(_tokens[_tokens.size() - 2] == ";" || _tokens[_tokens.size() - 2] == "("))
			return true;
		if (token == "\n") {
			token = _pToker->getTok();
			c = token;
			newline = 1;
		}
		else newline = 0;
		if (c == "#")
		{
			std::cout << c << std::endl;
			hash = 1;
			return true;
		}
	}
	return false;
}
size_t SemiExp::length()
{
	return _tokens.size();
}

std::string& SemiExp::operator[](int n)
{
	if (n < 0 || n >= (int)length())
		throw(std::invalid_argument("index out of range"));
	return _tokens[n];
}

std::string SemiExp::show(bool showNewLines)
{
	std::cout << "\n  ";
	for (auto token : _tokens)
		if (token != "\n")
			std::cout << token << " ";
	std::cout << "\n";
	return "";
}

bool SemiExp::remove(const std::string& tok)
{
	size_t index = find(tok);
	remove(index);
	return true;
}

bool SemiExp::remove(size_t i) {
	_tokens.erase(_tokens.begin() + i);
	return true;
}

size_t SemiExp::find(const std::string& tok) {
	size_t index = std::find(_tokens.begin(), _tokens.end(), tok) - _tokens.begin();
	return index;
}

void SemiExp::clear() {
	_tokens.clear();
}

bool SemiExp::merge(const std::string& firstTok, const std::string& secondTok) {
	return true;
}

void SemiExp::toLower() {
	for (auto token : _tokens) {
		std::transform(_tokens[0].begin(), _tokens[0].end(), _tokens.begin(), tolower);
	}
}
void SemiExp::trimFront() {

}

void SemiExp::push_back(const std::string& tok) {
	_tokens.push_back(tok);
}

#ifdef TEST_EXECUTIVE
int main()
{
	Toker toker;
	std::string fileSpec = "../Tokenizer/test.txt";
	std::fstream in(fileSpec);
	//Scanner::Toker::setSpecialSingleChars();
	//Scanner::Toker::setSpecialCharPairs();
	if (!in.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return 1;
	}
	toker.attach(&in);

	SemiExp semi(&toker);


	while (semi.get())
	{
		//std::cout << "\n  -- semiExpression --";
		ofs1 << "\n  -- semiExpression --";
		semi.show();
		if (!in.good())break;

	}
	/*
	   May have collected tokens, but reached end of stream
	   before finding SemiExp terminator.
	 */
	if (semi.length() > 0) {
		//std::cout << "\n  -- semiExpression --";
		ofs1 << "\n  -- semiExpression --";
		semi.show();
		//std::cout << "\n\n";
		ofs1 << "\n\n";
	}

	return 0;
}
#endif