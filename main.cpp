#include <iostream>
#include <vector>

using namespace std;

void report(unsigned line, string where, string message) {
    cerr << "[line " << line << "] Error" << where << ": " << message << endl;
}

void error(unsigned line, string message) {
    report(line, "", message);
}

class Token {
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
      And, Class, Else, False, Fun, For, IF, Nil, OR,
      Print, Return, Super, This, True, Var, While,

      Eof
    };

    Token(Type type, string lexeme, void *literal, unsigned line)
        : type(type)
        , lexeme(lexeme)
        , literal(literal)
        , line(line)
    { }

    Type type;
    string lexeme;
    void *literal;
    unsigned line;
};

class Scanner {
    public:
        using TokenList = vector<Token>;

        bool isAtEnd() {
            return current == end;
        }

        char advance() {
            char c = *current++;
            return c;
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

        void scanToken() {
            char c = advance();
            switch (c) {
                case '(': addToken(Token::LeftParen);       break;
                case ')': addToken(Token::RightParen);      break;
                case '{': addToken(Token::LeftBrace);       break;
                case '}': addToken(Token::RightBrace);      break;
                case ',': addToken(Token::Comma);           break;
                case '.': addToken(Token::Dot);             break;
                case '-': addToken(Token::Minus);           break;
                case '+': addToken(Token::Plus);            break;
                case ';': addToken(Token::Semicolon);       break;
                case '/': addToken(Token::Slash);           break;
                case '*': addToken(Token::Star);            break;
                default:    error(line, "Unexpected character"); break;
            }
        }

        void addToken(Token::Type type) { addToken(type, nullptr); }
        void addToken(Token::Type type, void *literal) {
            tokens.emplace_back(type, string{start, current}, literal, line);
        }

        TokenList complete() { return tokens; }

        TokenList tokens;
        string::iterator start;
        string::iterator current;
        string::iterator end;;
        int line = 1;
};

int main()
{
    string line;
    Scanner scanner{};

    while (getline(cin, line)) {
        scanner.addLine(line);
    }
    scanner.complete();

    return 0;
}

// vi: cin shiftwidth=4 tabstop=4 softtabstop=4 expandtab
