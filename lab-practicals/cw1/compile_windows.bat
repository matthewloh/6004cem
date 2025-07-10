@echo off
echo ========================================
echo OpenMP Coursework Compilation Script
echo ========================================

echo.
echo Checking for available compilers...

REM Check for GCC (MinGW)
where gcc >nul 2>&1
if %errorlevel% == 0 (
    echo GCC found - using GCC compiler
    set COMPILER=gcc
    set FLAGS=-fopenmp -O2 -Wall
    set EXT=.exe
    goto compile
)

REM Check for MSVC
where cl >nul 2>&1
if %errorlevel% == 0 (
    echo MSVC found - using Microsoft Visual C++ compiler
    set COMPILER=cl
    set FLAGS=/openmp /O2
    set EXT=.exe
    goto compile
)

echo ERROR: No suitable compiler found!
echo Please install one of the following:
echo   1. MinGW-w64 (includes GCC with OpenMP)
echo   2. Microsoft Visual Studio (includes MSVC with OpenMP)
echo   3. Intel C++ Compiler
pause
exit /b 1

:compile
echo.
echo Compiling with %COMPILER%...
echo.

if "%COMPILER%"=="gcc" (
    echo Compiling Part A...
    gcc %FLAGS% -o part_a_hello_openmp%EXT% part_a_hello_openmp.c -lm
    
    echo Compiling Part B...
    gcc %FLAGS% -o part_b_vector_addition%EXT% part_b_vector_addition.c -lm
    
    echo Compiling Part C...
    gcc %FLAGS% -o part_c_matrix_calculation%EXT% part_c_matrix_calculation.c -lm
) else (
    echo Compiling Part A...
    cl %FLAGS% part_a_hello_openmp.c /Fe:part_a_hello_openmp%EXT%
    
    echo Compiling Part B...
    cl %FLAGS% part_b_vector_addition.c /Fe:part_b_vector_addition%EXT%
    
    echo Compiling Part C...
    cl %FLAGS% part_c_matrix_calculation.c /Fe:part_c_matrix_calculation%EXT%
)

echo.
echo Compilation complete!
echo.
echo To run the programs:
echo   .\part_a_hello_openmp%EXT%
echo   .\part_b_vector_addition%EXT%
echo   .\part_c_matrix_calculation%EXT%
echo.
pause 