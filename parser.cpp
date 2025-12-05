#include "parser.h"
#include <stdexcept>
#include <iostream>

void Parser::eat(TokenType type) {
    if (currentToken().type == type) {
        pos++;
    } else {
        throw std::runtime_error("Syntax error: Unexpected token");
    }
}

bool Parser::match(TokenType type) {
    return currentToken().type == type;
}

bool Parser::matchAny(std::initializer_list<TokenType> types) {
    for (auto type : types) {
        if (match(type)) return true;
    }
    return false;
}

ASTNode* Parser::parsePrimary() {
    if (match(TokenType::NUM)) {
        ASTNode* node = new ASTNode(currentToken());
        eat(TokenType::NUM);
        return node;
    }
    else if (match(TokenType::STRING)) {
        ASTNode* node = new ASTNode(currentToken());
        eat(TokenType::STRING);
        return node;
    }
    else if (match(TokenType::IDENTIFIER)) {
        ASTNode* node = new ASTNode(currentToken());
        eat(TokenType::IDENTIFIER);
        return node;
    }
    else if (match(TokenType::LPAREN)) {
        eat(TokenType::LPAREN);
        ASTNode* expr = parseExpression();
        eat(TokenType::RPAREN);
        return expr;
    }
    else {
        throw std::runtime_error("Expected number, string, identifier, or '('");
    }
}

ASTNode* Parser::parseMultiplicative() {
    ASTNode* left = parsePrimary();
    
    while (matchAny({TokenType::MUL, TokenType::DIV, TokenType::GUN, TokenType::BHAG})) {
        Token op = currentToken();
        eat(op.type);
        ASTNode* right = parsePrimary();
        
        ASTNode* newNode = new ASTNode(op);
        newNode->left = left;
        newNode->right = right;
        left = newNode;
    }
    
    return left;
}

ASTNode* Parser::parseAdditive() {
    ASTNode* left = parseMultiplicative();
    
    while (matchAny({TokenType::PLUS, TokenType::MINUS, TokenType::JOG, TokenType::BIYOG})) {
        Token op = currentToken();
        eat(op.type);
        ASTNode* right = parseMultiplicative();
        
        ASTNode* newNode = new ASTNode(op);
        newNode->left = left;
        newNode->right = right;
        left = newNode;
    }
    
    return left;
}

ASTNode* Parser::parseComparison() {
    ASTNode* left = parseAdditive();
    
    if (matchAny({TokenType::LT, TokenType::GT, TokenType::EQ, TokenType::NEQ})) {
        Token op = currentToken();
        eat(op.type);
        ASTNode* right = parseAdditive();
        
        ASTNode* newNode = new ASTNode(op);
        newNode->left = left;
        newNode->right = right;
        left = newNode;
    }
    
    return left;
}

ASTNode* Parser::parseAssignment() {
    ASTNode* left = parseComparison();

    if (match(TokenType::ASSIGN)) {
        Token assignToken = currentToken();
        eat(TokenType::ASSIGN);
        ASTNode* right = parseAssignment();

        ASTNode* assignNode = new ASTNode(assignToken);
        assignNode->left = left;
        assignNode->right = right;
        return assignNode;
    }

    return left;
}

ASTNode* Parser::parseExpression() {
    return parseAssignment();
}

ASTNode* Parser::parsePrintStatement() {
    eat(TokenType::LEKHO);
    ASTNode* expr = parseExpression();
    eat(TokenType::SEMICOLON);
    
    ASTNode* node = new ASTNode(Token(TokenType::LEKHO));
    node->left = expr;
    return node;
}

ASTNode* Parser::parseVowelDetect() {
    eat(TokenType::SHOROBORNO);
    eat(TokenType::LPAREN);
    
    if (!match(TokenType::STRING)) {
        throw std::runtime_error("Expected string for vowel detection");
    }
    
    ASTNode* strNode = new ASTNode(currentToken());
    eat(TokenType::STRING);
    
    eat(TokenType::RPAREN);
    eat(TokenType::SEMICOLON);
    
    ASTNode* node = new ASTNode(Token(TokenType::SHOROBORNO));
    node->left = strNode;
    return node;
}

ASTNode* Parser::parseIfStatement() {
    eat(TokenType::JODI);
    eat(TokenType::LPAREN);
    
    ASTNode* condition = parseExpression();
    eat(TokenType::RPAREN);
    
    ASTNode* thenBlock = parseStatement();
    
    ASTNode* elseBlock = nullptr;
    if (match(TokenType::NAHOLE)) {
        eat(TokenType::NAHOLE);
        elseBlock = parseStatement();
    }
    
    ASTNode* node = new ASTNode(Token(TokenType::JODI));
    node->left = condition;
    node->right = thenBlock;
    node->extra = elseBlock;
    return node;
}

ASTNode* Parser::parseWhileStatement() {
    eat(TokenType::JOTOKKHON);
    eat(TokenType::LPAREN);
    
    ASTNode* condition = parseExpression();
    eat(TokenType::RPAREN);
    
    ASTNode* body = parseStatement();
    
    ASTNode* node = new ASTNode(Token(TokenType::JOTOKKHON));
    node->left = condition;
    node->right = body;
    return node;
}

ASTNode* Parser::parseForStatement() {
    eat(TokenType::PROTIBAR);
    eat(TokenType::LPAREN);

    // Simple for loop: for (init; condition; increment)
    ASTNode* init = parseExpression();
    eat(TokenType::SEMICOLON);

    ASTNode* condition = parseExpression();
    eat(TokenType::SEMICOLON);

    ASTNode* increment = parseExpression();
    eat(TokenType::RPAREN);

    ASTNode* body = parseStatement();

    // Create for node with body as child
    ASTNode* node = new ASTNode(Token(TokenType::PROTIBAR));
    node->left = init;
    node->right = condition;
    node->extra = increment;
    node->children.push_back(body);
    return node;
}

ASTNode* Parser::parseStatement() {
    if (match(TokenType::LEKHO)) {
        return parsePrintStatement();
    }
    else if (match(TokenType::SHOROBORNO)) {
        return parseVowelDetect();
    }
    else if (match(TokenType::JODI)) {
        return parseIfStatement();
    }
    else if (match(TokenType::JOTOKKHON)) {
        return parseWhileStatement();
    }
    else if (match(TokenType::PROTIBAR)) {
        return parseForStatement();
    }
    else if (match(TokenType::LBRACE)) {
        return parseBlock();
    }
    else {
        // Expression statement
        ASTNode* expr = parseExpression();
        eat(TokenType::SEMICOLON);
        return expr;
    }
}

ASTNode* Parser::parseBlock() {
    eat(TokenType::LBRACE);
    ASTNode* block = new ASTNode(Token(TokenType::LBRACE));
    
    while (!match(TokenType::RBRACE) && !match(TokenType::END)) {
        block->children.push_back(parseStatement());
    }
    
    eat(TokenType::RBRACE);
    return block;
}

ASTNode* Parser::parseProgram() {
    ASTNode* program = new ASTNode(Token(TokenType::LBRACE));
    
    while (!match(TokenType::END)) {
        program->children.push_back(parseStatement());
    }
    
    return program;
}