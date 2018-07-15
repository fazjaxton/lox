#ifndef _TOKEN_H
#define _TOKEN_H

#include <string>

/* Not sure where we're going with this "literal" stuff .. this works for now */
struct Value {
    std::string s;
    double d;
    bool isString = false; bool isDouble = false;

    Value(std::string s) : s(s),  d(0), isString(true) { }
    Value(double d) : s(""), d(d), isDouble(true) { }
    Value()         : s(""), d(0) { }

    std::string toString() const { return isDouble ? std::to_string(d) : s; }
};

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
      And, Class, Else, False, Fun, For, If, Nil, Or,
      Print, Return, Super, This, True, Var, While,

      Eof
    };

    Token(Type type, std::string lexeme, Value const & literal, unsigned line)
        : type(type)
        , lexeme(lexeme)
        , literal(literal)
        , line(line)
    { }

    std::string toString() const;

    Type type;
    std::string lexeme;
    Value literal;
    unsigned line;

private:
    static const std::string names[];
};

#endif
