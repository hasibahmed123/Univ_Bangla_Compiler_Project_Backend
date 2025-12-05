@echo off
chcp 65001 > nul
cls

echo ========================================
echo    বাংলা কম্পাইলার - Bangla Compiler
echo ========================================
echo.

echo কম্পাইল করছি...
g++ -o bangla_compiler main.cpp lexer.cpp parser.cpp evaluator.cpp -std=c++11 -static-libgcc -static-libstdc++

if %errorlevel% equ 0 (
    echo কম্পাইল সফল!
    echo.
    echo ========================================
    bangla_compiler.exe
) else (
    echo কম্পাইল ব্যর্থ!
)

echo.
pause