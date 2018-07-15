#include "error.h"
#include "scanner.h"

using namespace std;

void Scanner::scan(string &s) {
    start = current = s.begin();
    end = s.end();
    scanTokens();
}


void Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }
}


void Scanner::skipRestOfLine() {
    while (!isAtEnd() && peek() != '\n')
        advance();
}


void Scanner::appendCurrentString() {
    currentString.append(start, current);
}


void Scanner::doString() {
    inString = true;

    while (advance() != '"') {
        if (isAtEnd()) {
            appendCurrentString();
            return;
        }
    }

    // Multiline strings not quite working...
    appendCurrentString();
    addToken(Token::String, string(currentString.begin() + 1, currentString.end() - 1));
    inString = false;
    currentString = "";
}


void Scanner::doNumber() {
    while (isDigit(peek()))
        advance();
    if (peek() == '.' && isDigit(peekNext())) {
        advance();
        while (isDigit(peek()))
            advance();
    }

    addToken(Token::Number, stod(string(start, current)));
}


void Scanner::doIdentifier() {
    while (isAlphanumeric(peek()))
        advance();

    string word{start, current};
    auto keyword = reservedWords.find(word);

    if (keyword != reservedWords.end()) {
        addToken(keyword->second);
    }
    else {
        addToken(Token::Identifier);
    }
}


void Scanner::doBlockComment() {
    inComment = true;
    for (;;) {
        while (advance() != '*') {
            if (isAtEnd()) return;
        }
        if (match('/')) {
            inComment = false;
            return;
        }
    }
}


bool Scanner::isComment() {
    bool res = true;
    if (match('/')) {
        skipRestOfLine();
    } else if (match('*')) {
        doBlockComment();
    } else {
        res = false;
    }

    return res;
}


void Scanner::scanToken() {
    if (inString) {
        doString();
        return;
    }
    if (inComment) {
        doBlockComment();
        return;
    }

    char c = advance();
    switch (c) {
        using T = Token;
        /* Single character tokens */
        case '(': addToken(T::LeftParen);       break;
        case ')': addToken(T::RightParen);      break;
        case '{': addToken(T::LeftBrace);       break;
        case '}': addToken(T::RightBrace);      break;
        case ',': addToken(T::Comma);           break;
        case '.': addToken(T::Dot);             break;
        case '-': addToken(T::Minus);           break;
        case '+': addToken(T::Plus);            break;
        case '*': addToken(T::Star);            break;
        case ';': addToken(T::Semicolon);       break;

                  /* Single or double character tokens */
        case '/': if (!isComment()) addToken(T::Slash);                 break;
        case '!': addToken(match('=') ? T::Bang    : T::BangEqual);     break;
        case '=': addToken(match('=') ? T::Equal   : T::EqualEqual);    break;
        case '>': addToken(match('=') ? T::Greater : T::GreaterEqual);  break;
        case '<': addToken(match('=') ? T::Less    : T::LessEqual);     break;

                  /* Whitespace */
        case ' ':                                                       break;
        case '\r':                                                      break;
        case '\t':                                                      break;
        case '\n': line++;                                              break;

                   /* Literals */
        case '"': doString(); break;

        default:
                  if (isDigit(c)) {
                      doNumber();
                  }
                  else if (isAlpha(c)) {
                      doIdentifier();
                  }
                  else {
                      error(line, "Unexpected character"); break;
                  }
    }
}


void Scanner::addToken(Token::Type type, Value const & literal) {
    tokens.emplace_back(type, string{start, current}, literal, line);
}


const Scanner::TokenMap Scanner::reservedWords = {
    Scanner::TokenMap::value_type{"and",        Token::And      },
    Scanner::TokenMap::value_type{"class",      Token::Class    },
    Scanner::TokenMap::value_type{"else",       Token::Else     },
    Scanner::TokenMap::value_type{"false",      Token::False    },
    Scanner::TokenMap::value_type{"fun",        Token::Fun      },
    Scanner::TokenMap::value_type{"for",        Token::For      },
    Scanner::TokenMap::value_type{"if",         Token::If       },
    Scanner::TokenMap::value_type{"nil",        Token::Nil      },
    Scanner::TokenMap::value_type{"or",         Token::Or       },
    Scanner::TokenMap::value_type{"print",      Token::Print    },
    Scanner::TokenMap::value_type{"return",     Token::Return   },
    Scanner::TokenMap::value_type{"super",      Token::Super    },
    Scanner::TokenMap::value_type{"this",       Token::This     },
    Scanner::TokenMap::value_type{"true",       Token::True     },
    Scanner::TokenMap::value_type{"var",        Token::Var      },
    Scanner::TokenMap::value_type{"while",      Token::While    },
};
