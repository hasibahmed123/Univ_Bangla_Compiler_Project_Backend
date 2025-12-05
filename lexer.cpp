#include "lexer.h"
#include <cctype>
#include <iostream>
#include <algorithm>

void Lexer::skipWhitespace() {
    while (pos < input.length() && std::isspace(static_cast<unsigned char>(input[pos]))) {
        pos++;
    }
}

bool Lexer::startsWith(const std::string& prefix) {
    if (pos + prefix.length() > input.length()) return false;
    return input.compare(pos, prefix.length(), prefix) == 0;
}

Token Lexer::readBanglaWord() {
    size_t start = pos;
    
    // Read until whitespace or special character
    while (pos < input.length()) {
        unsigned char c = static_cast<unsigned char>(input[pos]);
        
        // Stop at whitespace or special characters
        if (std::isspace(c) || 
            c == '(' || c == ')' || c == '{' || c == '}' ||
            c == ';' || c == '"' || c == '+' || c == '-' ||
            c == '*' || c == '/' || c == '=' || c == '!' ||
            c == '<' || c == '>') {
            break;
        }
        
        // Skip Bangla/Unicode characters (3 bytes each)
        if (c >= 0xE0) { // UTF-8 multi-byte start
            if (c == 0xE0 && pos + 2 < input.length()) {
                pos += 3; // Skip UTF-8 character
            } else {
                pos++; // Safety
            }
        } else {
            pos++; // ASCII
        }
    }
    
    std::string word = input.substr(start, pos - start);
    
    // Map ACTUAL BANGLA words to tokens
    static std::map<std::string, TokenType> banglaKeywords = {
        // Numbers (Bangla)
        {"এক", TokenType::NUM},
        {"দুই", TokenType::NUM},
        {"তিন", TokenType::NUM},
        {"চার", TokenType::NUM},
        {"পাঁচ", TokenType::NUM},
        
        // Arithmetic (Bangla)
        {"যোগ", TokenType::JOG},
        {"বিয়োগ", TokenType::BIYOG},
        {"গুণ", TokenType::GUN},
        {"ভাগ", TokenType::BHAG},
        
        // Control flow (Bangla)
        {"যদি", TokenType::JODI},
        {"নাহলে", TokenType::NAHOLE},
        {"যতক্ষণ", TokenType::JOTOKKHON},
        {"প্রতিবার", TokenType::PROTIBAR},
        
        // Functions (Bangla)
        {"লেখ", TokenType::LEKHO},
        {"স্বরবর্ণচেক", TokenType::SHOROBORNO}
    };
    
    // Check for Bangla keywords
    auto it = banglaKeywords.find(word);
    if (it != banglaKeywords.end()) {
        // Set numeric values for Bangla numbers
        if (word == "এক") return Token(TokenType::NUM, 1);
        else if (word == "দুই") return Token(TokenType::NUM, 2);
        else if (word == "তিন") return Token(TokenType::NUM, 3);
        else if (word == "চার") return Token(TokenType::NUM, 4);
        else if (word == "পাঁচ") return Token(TokenType::NUM, 5);
        else return Token(it->second);
    }

    // Check for Bangla digits
    if (word == "০") return Token(TokenType::NUM, 0);
    else if (word == "১") return Token(TokenType::NUM, 1);
    else if (word == "২") return Token(TokenType::NUM, 2);
    else if (word == "৩") return Token(TokenType::NUM, 3);
    else if (word == "৪") return Token(TokenType::NUM, 4);
    else if (word == "৫") return Token(TokenType::NUM, 5);
    else if (word == "৬") return Token(TokenType::NUM, 6);
    else if (word == "৭") return Token(TokenType::NUM, 7);
    else if (word == "৮") return Token(TokenType::NUM, 8);
    else if (word == "৯") return Token(TokenType::NUM, 9);
    
    // Check if it's a regular number
    bool isNumber = true;
    for (char c : word) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            isNumber = false;
            break;
        }
    }
    if (isNumber) {
        return Token(TokenType::NUM, std::stoi(word));
    }
    
    return Token(TokenType::IDENTIFIER, word);
}

Token Lexer::readNumber() {
    size_t start = pos;
    while (pos < input.length() && std::isdigit(static_cast<unsigned char>(input[pos]))) {
        pos++;
    }
    std::string numStr = input.substr(start, pos - start);
    return Token(TokenType::NUM, std::stoi(numStr));
}

Token Lexer::readString() {
    pos++; // Skip opening quote
    size_t start = pos;
    while (pos < input.length() && input[pos] != '"') {
        pos++;
    }
    std::string str = input.substr(start, pos - start);
    pos++; // Skip closing quote
    return Token(TokenType::STRING, str);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;
    
    while (pos < input.length()) {
        skipWhitespace();
        if (pos >= input.length()) break;
        
        unsigned char current = static_cast<unsigned char>(input[pos]);
        
        if (current == '"') {
            tokens.push_back(readString());
        }
        else if (std::isdigit(current)) {
            tokens.push_back(readNumber());
        }
        else if (current >= 0xE0 || std::isalpha(current)) {
            // Bangla or English word
            tokens.push_back(readBanglaWord());
        }
        else {
            // Handle symbols
            switch (current) {
                case '+': tokens.push_back(Token(TokenType::PLUS)); pos++; break;
                case '-': tokens.push_back(Token(TokenType::MINUS)); pos++; break;
                case '*': tokens.push_back(Token(TokenType::MUL)); pos++; break;
                case '/': tokens.push_back(Token(TokenType::DIV)); pos++; break;
                case '(': tokens.push_back(Token(TokenType::LPAREN)); pos++; break;
                case ')': tokens.push_back(Token(TokenType::RPAREN)); pos++; break;
                case '{': tokens.push_back(Token(TokenType::LBRACE)); pos++; break;
                case '}': tokens.push_back(Token(TokenType::RBRACE)); pos++; break;
                case ';': tokens.push_back(Token(TokenType::SEMICOLON)); pos++; break;
                case '=':
                    if (pos + 1 < input.length() && input[pos + 1] == '=') {
                        tokens.push_back(Token(TokenType::EQ));
                        pos += 2;
                    } else {
                        tokens.push_back(Token(TokenType::ASSIGN));
                        pos++;
                    }
                    break;
                case '!': 
                    if (pos + 1 < input.length() && input[pos + 1] == '=') {
                        tokens.push_back(Token(TokenType::NEQ));
                        pos += 2;
                    } else {
                        tokens.push_back(Token(TokenType::INVALID));
                        pos++;
                    }
                    break;
                case '<': tokens.push_back(Token(TokenType::LT)); pos++; break;
                case '>': tokens.push_back(Token(TokenType::GT)); pos++; break;
                default: 
                    tokens.push_back(Token(TokenType::INVALID));
                    pos++;
                    break;
            }
        }
    }
    
    tokens.push_back(Token(TokenType::END));
    return tokens;
}