#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>

enum class TokenType {
    // Basic tokens
    NUM, PLUS, MINUS, MUL, DIV, 
    LPAREN, RPAREN, LBRACE, RBRACE,
    SEMICOLON, END, INVALID,
    
    // Bangla numbers (actual Bangla)
    BANGLA_NUM,
    
    // Bangla Arithmetic (actual Bangla)
    JOG,     // যোগ
    BIYOG,   // বিয়োগ
    GUN,     // গুণ
    BHAG,    // ভাগ
    
    // Bangla Control Flow (actual Bangla)
    JODI,    // যদি
    NAHOLE,  // নাহলে
    JOTOKKHON, // যতক্ষণ
    PROTIBAR, // প্রতিবার
    
    // Bangla Functions (actual Bangla)
    LEKHO,   // লেখ
    SHOROBORNO, // স্বরবর্ণচেক
    
    // Comparison
    EQ, NEQ, LT, GT,

    // Assignment
    ASSIGN,

    // Others
    STRING, IDENTIFIER
};

struct Token {
    TokenType type;
    int value;
    std::string strValue;
    
    Token(TokenType t, int v = 0) : type(t), value(v) {}
    Token(TokenType t, std::string s) : type(t), strValue(s) {}
};

class Lexer {
    std::string input;
    size_t pos = 0;
    
    void skipWhitespace();
    Token readBanglaWord();
    Token readNumber();
    Token readString();
    bool startsWith(const std::string& prefix);
    
public:
    Lexer(const std::string& in) : input(in) {}
    std::vector<Token> tokenize();
};