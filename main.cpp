#include <iostream>
#include "lexer.h"
#include "parser.h"
#include "evaluator.h"

#ifdef _WIN32
#include <windows.h>
#endif

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    #endif
    
    std::cout << "======================================\n";
    std::cout << "       বাংলা প্রোগ্রামিং কম্পাইলার\n";
    std::cout << "======================================\n\n";
    
    std::cout << "সমর্থিত বৈশিষ্ট্য:\n";
    std::cout << "1. গাণিতিক: যোগ, বিয়োগ, গুণ, ভাগ\n";
    std::cout << "2. যদি-নাহলে: যদি (শর্ত) { ... } নাহলে { ... }\n";
    std::cout << "3. লুপ: যতক্ষণ (শর্ত) { ... }\n";
    std::cout << "4. লুপ: প্রতিবার (শুরু; শর্ত; পরিবর্তন) { ... }\n";
    std::cout << "5. স্বরবর্ণচেক: স্বরবর্ণচেক(\"পাঠ্য\")\n";
    std::cout << "6. লেখ: লেখ প্রকাশ্য;\n\n";
    
    // Test all features
    std::string tests[] = {
        // 1. Bangla Arithmetic
        "লেখ দুই যোগ তিন;",
        
        // 2. Bangla If-Else
        "যদি (এক < দুই) { লেখ \"ছোট\"; } নাহলে { লেখ \"বড়\"; }",
        
        // 3. Bangla While Loop
        "x = 0; যতক্ষণ (x < ৩) { লেখ x; x = x + ১; }",
        
        // 4. Bangla For Loop
        "প্রতিবার (i = ০; i < ৩; i = i + ১) { লেখ i; }",
        
        // 5. Bangla Vowel Detection
        "স্বরবর্ণচেক(\"আমি\");"
    };
    
    std::string descriptions[] = {
        "গাণিতিক (যোগ)",
        "যদি-নাহলে",
        "যতক্ষণ লুপ",
        "প্রতিবার লুপ", 
        "স্বরবর্ণচেক"
    };
    
    for (int i = 0; i < 5; i++) {
        std::cout << "\nপরীক্ষা " << (i+1) << ": " << descriptions[i] << "\n";
        std::cout << "ইনপুট: " << tests[i] << "\n";
        std::cout << "আউটপুট:\n";
        std::cout << "-------------------\n";
        
        try {
            Lexer lexer(tests[i]);
            auto tokens = lexer.tokenize();
            
            Parser parser(tokens);
            ASTNode* tree = parser.parseProgram();
            
            Evaluator eval;
            eval.executeProgram(tree);
            
            delete tree;
            
        } catch (const std::exception& e) {
            std::cout << "ত্রুটি: " << e.what() << "\n";
        }
    }
    
    // User input
    std::cout << "\n\nআপনার বাংলা কোড লিখুন:\n";
    std::cout << "উদাহরণ: লেখ দুই যোগ তিন;\n";
    std::cout << "ইনপুট: ";
    
    std::string userInput;
    std::getline(std::cin, userInput);
    
    if (!userInput.empty()) {
        try {
            Lexer lexer(userInput);
            auto tokens = lexer.tokenize();
            
            Parser parser(tokens);
            ASTNode* tree = parser.parseProgram();
            
            Evaluator eval;
            std::cout << "\nফলাফল:\n";
            eval.executeProgram(tree);
            
            delete tree;
            
        } catch (const std::exception& e) {
            std::cout << "ত্রুটি: " << e.what() << "\n";
        }
    }
    
    std::cout << "\nপ্রোগ্রাম শেষ।\n";
    return 0;
}