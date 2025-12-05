#pragma once
#include "lexer.h"
#include <vector>
#include <stdexcept>

struct ASTNode {
    Token token;
    ASTNode* left;
    ASTNode* right;
    ASTNode* extra;  // For else, increment, etc.
    std::vector<ASTNode*> children;  // For block statements
    
    ASTNode(Token t) : token(t), left(nullptr), right(nullptr), extra(nullptr) {}
    ~ASTNode() {
        delete left;
        delete right;
        delete extra;
        for (auto child : children) delete child;
    }
};

class Parser {
    std::vector<Token> tokens;
    size_t pos = 0;

    Token currentToken() { return tokens[pos]; }
    void eat(TokenType type);
    bool match(TokenType type);
    bool matchAny(std::initializer_list<TokenType> types);
    
    // Parsing methods
    ASTNode* parsePrimary();
    ASTNode* parseMultiplicative();
    ASTNode* parseAdditive();
    ASTNode* parseComparison();
    ASTNode* parseAssignment();
    ASTNode* parseExpression();
    
    // Statement parsing
    ASTNode* parseStatement();
    ASTNode* parseBlock();
    ASTNode* parsePrintStatement();
    ASTNode* parseVowelDetect();
    ASTNode* parseIfStatement();
    ASTNode* parseWhileStatement();
    ASTNode* parseForStatement();
    
public:
    Parser(const std::vector<Token>& toks) : tokens(toks) {}
    ASTNode* parseProgram();
};