#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

void report(unsigned line, string where, string message) {
    cerr << "[line " << line << "] Error" << where << ": " << message << endl;
}

void error(unsigned line, string message) {
    report(line, "", message);
}

struct Value {
    string s;
    double d;
    bool isString = false; bool isDouble = false;

    Value(string s) : s(s),  d(0), isString(true) { }
    Value(double d) : s(""), d(d), isDouble(true) { }
    Value()         : s(""), d(0) { }
};

class Token {
    static const string names[];

    public:
    enum Type {
      // Single-character tokens.
      LeftParen, RightParen, LeftBrace, RightBrace,
      Comma, Dot, Minus, Plus, Semicolon, Slash, Star,

      // One or two character tokens.
      Bang, BangEqual,
      Equal, EqualEqual,
      Greater, GreaterEqual,
      Less, LessEqual,

      // Literals.
      Identifier, String, Number,

      // Keywords.
      And, Class, Else, False, Fun, For, If, Nil, Or,
      Print, Return, Super, This, True, Var, While,

      Eof
    };

    Token(Type type, string lexeme, Value const & literal, unsigned line)
        : type(type)
        , lexeme(lexeme)
        , literal(literal)
        , line(line)
    { }

    string toString() const {
        return names[type] + " " + lexeme + " (" + (literal.isString ? literal.s : (literal.isDouble ? to_string(literal.d) : "")) + ") at " + std::to_string(line);
    }

    Type type;
    string lexeme;
    Value literal;
    unsigned line;
};

class Scanner {
    static const char InvalidChar = 255;
    public:
        using TokenList = vector<Token>;
        using TokenMap  = std::unordered_map<string, Token::Type>;

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

        void addLine(string &s) {
            start = current = s.begin();
            end = s.end();
            scanTokens();
            line++;
        }

        void scanTokens() {
            while (!isAtEnd()) {
                start = current;
                scanToken();
            }
        }

        void skipRestOfLine() {
            while (!isAtEnd() && peek() != '\n')
                advance();
        }

        void appendCurrentString() {
            currentString.append(start, current);
        }

        bool isDigit(char c) {
            return c >= '0' && c <= '9';
        }

        bool isAlpha(char c) {
            return c >= 'A' && c <= 'Z'
                || c >= 'a' && c <= 'z'
                || c == '_';
        }

        bool isAlphanumeric(char c) {
            return isDigit(c) || isAlpha(c);
        }

        void doString() {
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

        void doNumber() {
            while (isDigit(peek()))
                advance();
            if (peek() == '.' && isDigit(peekNext())) {
                advance();
                while (isDigit(peek()))
                    advance();
            }

            addToken(Token::Number, stod(string(start, current)));
        }

        void doIdentifier() {
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

        void doBlockComment() {
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

        bool isComment() {
            if (match('/'))
                skipRestOfLine();
            else if (match('*'))
                doBlockComment();
        }

        void scanToken() {
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

        void addToken(Token::Type type) { addToken(type, Value()); }
        void addToken(Token::Type type, Value const & literal) {
            tokens.emplace_back(type, string{start, current}, literal, line);
        }

        TokenList complete() { return tokens; }

        TokenList tokens;
        static const TokenMap reservedWords;
        string::iterator start;
        string::iterator current;
        string::iterator end;;
        int line = 1;
        bool inString = false;
        bool inComment = false;
        string currentString = "";
};

int main()
{
    string line;
    Scanner scanner{};

    while (getline(cin, line)) {
        scanner.addLine(line);
    }
    auto tokens = scanner.complete();
    for (auto const & token : tokens) {
        cout << token.toString() << endl;
    }

    return 0;
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

const string Token::names[] = {
    "LeftParen",
    "RightParen",
    "LeftBrace",
    "RightBrace",
    "Comma",
    "Dot",
    "Minus",
    "Plus",
    "Semicolon",
    "Slash",
    "Star",
    "Bang",
    "BangEqual",
    "Equal",
    "EqualEqual",
    "Greater",
    "GreaterEqual",
    "Less",
    "LessEqual",
    "Identifier",
    "String",
    "Number",
    "And",
    "Class",
    "Else",
    "False",
    "Fun",
    "For",
    "If",
    "Nil",
    "Or",
    "Print",
    "Return",
    "Super",
    "This",
    "True",
    "Var",
    "While",
    "Eof"
};
// vi: cin shiftwidth=4 tabstop=4 softtabstop=4 expandtab
