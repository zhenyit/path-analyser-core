#include "stdafx.h"
#include"lexical.h"
namespace nsFitness
{
	void lexicalAnalysis(std::vector<SToken>&VToken, const std::string&expression)
	{
		//static char LexE[] = "Lexical Error";
		size_t i, symbolLength = 0;
		SToken token;
		for (i = 0; expression[i] != '\0'; ++i)
		{
			switch (expression[i])
			{
			case ' ':
			case '\n':
			case '\t':
			case '\r':
				if (symbolLength) {
					token.tt = token.NumExpr;
					token.str = expression.substr(i - symbolLength, symbolLength);
					VToken.push_back(token);
					symbolLength = 0;
				}
				break;
			case '|':
				if ('|' == expression[i + 1]) {
					if (symbolLength) {
						token.tt = token.NumExpr;
						token.str = expression.substr(i - symbolLength, symbolLength);
						VToken.push_back(token);
						symbolLength = 0;
					}
					++i;
					token.tt = token.Or;
					token.str = "||";
					VToken.push_back(token);
				}
				else ++symbolLength;
				break;
			case '&':
				if ('&' == expression[i + 1]) {
					if (symbolLength) {
						token.tt = token.NumExpr;
						token.str = expression.substr(i - symbolLength, symbolLength);
						VToken.push_back(token);
						symbolLength = 0;
					}
					++i;
					token.tt = token.And;
					token.str = "&&";
					VToken.push_back(token);
				}
				else ++symbolLength;
				break;
			case '=':
				if ('=' == expression[i + 1]) {
					if (symbolLength) {
						token.tt = token.NumExpr;
						token.str = expression.substr(i - symbolLength, symbolLength);
						VToken.push_back(token);
						symbolLength = 0;
					}
					++i;
					token.tt = token.RelaOp;
					token.str = "==";
					VToken.push_back(token);
				}
				else ++symbolLength;
				break;
			case '!':
				if (symbolLength) {
					token.tt = token.NumExpr;
					token.str = expression.substr(i - symbolLength, symbolLength);
					VToken.push_back(token);
					symbolLength = 0;
				}
				if ('=' == expression[i + 1]) {
					++i;
					token.tt = token.RelaOp;
					token.str = "!=";
					VToken.push_back(token);
				}
				else {
					token.tt = token.Not;
					token.str = "!";
					VToken.push_back(token);
				}
				break;
			case '>':
				if (symbolLength) {
					token.tt = token.NumExpr;
					token.str = expression.substr(i - symbolLength, symbolLength);
					VToken.push_back(token);
					symbolLength = 0;
				}
				if ('=' == expression[i + 1]) {
					++i;
					token.str = ">=";
				}
				else {
					token.str = ">";
				}
				token.tt = token.RelaOp;
				VToken.push_back(token);
				break;
			case '<':
				if (symbolLength) {
					token.tt = token.NumExpr;
					token.str = expression.substr(i - symbolLength, symbolLength);
					VToken.push_back(token);
					symbolLength = 0;
				}
				if ('=' == expression[i + 1]) {
					++i;
					token.str = "<=";
				}
				else token.str = "<";
				token.tt = token.RelaOp;
				VToken.push_back(token);
				break;
			case '(':
				if (symbolLength) {
					token.tt = token.NumExpr;
					token.str = expression.substr(i - symbolLength, symbolLength);
					VToken.push_back(token);
					symbolLength = 0;
				}
				token.tt = token.LeftBracket;
				token.str = "(";
				VToken.push_back(token);
				break;
			case ')':
				if (symbolLength) {
					token.tt = token.NumExpr;
					token.str = expression.substr(i - symbolLength, symbolLength);
					VToken.push_back(token);
					symbolLength = 0;
				}
				token.tt = token.RightBracket;
				token.str = ")";
				VToken.push_back(token);
				break;
			default:
				++symbolLength;
			}
		}
		if (symbolLength) {
			token.tt = token.NumExpr;
			token.str = expression.substr(i - symbolLength, symbolLength);
			VToken.push_back(token);
			symbolLength = 0;
		}
	}
}
