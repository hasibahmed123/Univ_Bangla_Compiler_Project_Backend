#include "evaluator.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>

bool Evaluator::isBanglaVowel(const std::string& str) {
    // Bangla vowels in UTF-8
    std::vector<std::string> banglaVowels = {
        "অ", "আ", "ই", "ঈ", "উ", "ঊ", 
        "ঋ", "এ", "ঐ", "ও", "ঔ"
    };
    
    for (const auto& vowel : banglaVowels) {
        if (str.find(vowel) != std::string::npos) {
            return true;
        }
    }
    return false;
}

bool Evaluator::evaluateCondition(ASTNode* node) {
    if (!node) return false;
    
    int leftVal = evaluate(node->left);
    int rightVal = evaluate(node->right);
    
    switch (node->token.type) {
        case TokenType::EQ: return leftVal == rightVal;
        case TokenType::NEQ: return leftVal != rightVal;
        case TokenType::LT: return leftVal < rightVal;
        case TokenType::GT: return leftVal > rightVal;
        default: return false;
    }
}

void Evaluator::executeBlock(ASTNode* node) {
    if (!node) return;
    
    for (ASTNode* child : node->children) {
        evaluate(child);
    }
}

int Evaluator::evaluate(ASTNode* node) {
    if (!node) return 0;

    switch (node->token.type) {
        case TokenType::NUM:
            return node->token.value;

        case TokenType::STRING:
            // Return length for now
            return node->token.strValue.length();

        case TokenType::IDENTIFIER:
            return getVariable(node->token.strValue);

        // Arithmetic operations
        case TokenType::PLUS:
        case TokenType::JOG:
            return evaluate(node->left) + evaluate(node->right);

        case TokenType::MINUS:
        case TokenType::BIYOG:
            return evaluate(node->left) - evaluate(node->right);

        case TokenType::MUL:
        case TokenType::GUN:
            return evaluate(node->left) * evaluate(node->right);

        case TokenType::DIV:
        case TokenType::BHAG:
            {
                int rightVal = evaluate(node->right);
                if (rightVal == 0) throw std::runtime_error("Division by zero");
                return evaluate(node->left) / rightVal;
            }

        case TokenType::ASSIGN:
            {
                int val = evaluate(node->right);
                setVariable(node->left->token.strValue, val);
                return val;
            }

        case TokenType::LEKHO:
            {
                ASTNode* expr = node->left;
                if (expr->token.type == TokenType::STRING) {
                    std::cout << "লেখ: " << expr->token.strValue << std::endl;
                } else {
                    int val = evaluate(expr);
                    std::cout << "লেখ: " << val << std::endl;
                }
                return 0;
            }

        case TokenType::SHOROBORNO:
            {
                std::string str = node->left->token.strValue;
                bool hasVowel = isBanglaVowel(str);
                std::cout << "স্বরবর্ণ আছে: " << (hasVowel ? "হ্যাঁ" : "না") << std::endl;
                return hasVowel ? 1 : 0;
            }

        case TokenType::JODI: // যদি (if)
            {
                if (evaluateCondition(node->left)) {
                    executeBlock(node->right);
                } else if (node->extra) {
                    executeBlock(node->extra);
                }
                return 0;
            }

        case TokenType::JOTOKKHON: // যতক্ষণ (while)
            {
                while (evaluateCondition(node->left)) {
                    executeBlock(node->right);
                }
                return 0;
            }

        case TokenType::PROTIBAR: // প্রতিবার (for)
            {
                // init
                if (node->left) evaluate(node->left);

                // condition loop
                while (node->right && evaluateCondition(node->right)) {
                    // body (first child of wrapper)
                    if (!node->children.empty()) {
                        for (ASTNode* child : node->children) {
                            evaluate(child);
                        }
                    }

                    // increment
                    if (node->extra) evaluate(node->extra);
                }
                return 0;
            }

        case TokenType::LBRACE:
            executeBlock(node);
            return 0;

        default:
            throw std::runtime_error("Unknown node type");
    }
}

void Evaluator::executeProgram(ASTNode* node) {
    evaluate(node);
}

void Evaluator::setVariable(const std::string& name, int value) {
    variables[name] = value;
}

int Evaluator::getVariable(const std::string& name) {
    if (variables.find(name) == variables.end()) {
        return 0; // Default to 0 if not set
    }
    return variables[name];
}
