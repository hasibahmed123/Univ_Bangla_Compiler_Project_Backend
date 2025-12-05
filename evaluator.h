#pragma once
#include "parser.h"
#include <unordered_map>
#include <string>

class Evaluator {
    std::unordered_map<std::string, int> variables;

    bool isBanglaVowel(const std::string& str);
    bool evaluateCondition(ASTNode* node);
    void executeBlock(ASTNode* node);

public:
    int evaluate(ASTNode* node);
    void executeProgram(ASTNode* node);
    void setVariable(const std::string& name, int value);
    int getVariable(const std::string& name);
};
