#ifndef _SCANNER_H
#define _SCANNER_H

#include "token.h"

#include <unordered_map>
#include <vector>

class Scanner {
    public:
        using TokenList = std::vector<Token>;
        using TokenMap  = std::unordered_map<std::string, Token::Type>;

        void scan(std::string &s);
        void incLine() { line++; }
        TokenList complete() { return tokens; }

    private:
        static const char InvalidChar = 255;

        bool isAtEnd()      { return current == end; }
        bool isNextEnd()    { return (current == end) || (current + 1 == end); }
        char advance()      { return isAtEnd()   ? InvalidChar : *current++; }
        char peek()         { return isAtEnd()   ? InvalidChar : *current; }
        char peekNext()     { return isNextEnd() ? InvalidChar : *(current + 1); }

        bool match(char c) {
            if (current == end || *current != c)
                return false;

            current++;
            return true;
        }

        bool isDigit(char c)        { return (c >= '0' && c <= '9'); }
        bool isAlpha(char c)        { return (c >= 'A' && c <= 'Z')
                                          || (c >= 'a' && c <= 'z')
                                          || ( c == '_'); }
        bool isAlphanumeric(char c) { return isDigit(c) || isAlpha(c); }

        void scanTokens();
        void skipRestOfLine();
        void appendCurrentString();
        void doString();
        void doNumber();
        void doIdentifier();
        void doBlockComment();
        bool isComment();
        void scanToken();
        void addToken(Token::Type type) { addToken(type, Value()); }
        void addToken(Token::Type type, Value const & literal);

        TokenList tokens;
        static const TokenMap reservedWords;

	std::string::iterator start;
	std::string::iterator current;
	std::string::iterator end;;

        int line = 1;
        bool inString = false;
        bool inComment = false;
	std::string currentString = "";
};

#endif
