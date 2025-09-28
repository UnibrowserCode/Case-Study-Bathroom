@echo off
g++ -std=c++17 -Wall -Wextra -I./src/lib -o bathroom_app src/main.cpp src/lib/*.cpp

echo 

cmd /k bathroom_app.exe
