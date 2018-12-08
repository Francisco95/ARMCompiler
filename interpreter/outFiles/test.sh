#!/bin/bash
#Run this in terminal
#+ Command to compile assembly program
echo "running basic int const test..."
as -o basic_int_const.o basic_int_const.s
gcc -o out basic_int_const.o
./out
rm out

echo "running basic add test..."
as -o basic_add.o basic_add.s 
gcc -o out basic_add.o
./out
rm out

echo "running basic sub test..."
as -o basic_sub.o basic_sub.s
gcc -o out basic_sub.o
./out
rm out

echo "running basic if test..."
as -o basic_if.o basic_if.s
gcc -o out basic_if.o
./out
rm out

echo "running basic fun test..."
as -o basic_fun.o basic_fun.s
gcc -o out basic_fun.o
./out
rm out

echo "running simple test..."
as -o simple.o simple.s
gcc -o out simple.o
./out
rm out

echo "running complex test..."
as -o complex.o complex.s
gcc -o out complex.o
./out
rm out

rm *.o
  
