@echo off
g++ -std=c++17 -Wall -Wextra -I./src/lib ^
    -Ofast -march=native -flto -funroll-loops ^
    -ftree-vectorize -ftree-loop-distribute-patterns -ftree-loop-vectorize ^
    -fomit-frame-pointer -fno-exceptions -fstrict-aliasing -Wno-unused-result ^
    -floop-parallelize-all -ffast-math -fno-math-errno ^
    -fno-rounding-math -flifetime-dse -fbranch-target-load-optimize ^
    -fprefetch-loop-arrays ^
    -o bathroom_app src/main.cpp

echo.
cmd /k bathroom_app.exe
