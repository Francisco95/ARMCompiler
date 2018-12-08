#!/bin/bash
#Run this in terminal
#+ Command to compile c++ program. here i used common one
echo "Compiling compiler.cpp"
g++ compiler.cpp -o compiler -lsexp
echo "Executing on test cases:"
./compiler ./inFiles/basic_int_const.txt ./outFiles/basic_int_const.s
echo "Compilation for basic test INT_CONST done"
./compiler ./inFiles/basic_add.txt ./outFiles/basic_add.s
echo "Compilation for basic test ADD done"
./compiler ./inFiles/basic_sub.txt ./outFiles/basic_sub.s
echo "Compilation for basic test SUB done"
./compiler ./inFiles/basic_if.txt ./outFiles/basic_if.s
echo "Compilation for basic test IF0 done"
./compiler ./inFiles/basic_fun.txt ./outFiles/basic_fun.s
echo "Compilation for basic test FUN done"
./compiler ./inFiles/simple.txt ./outFiles/simple.s
echo "Compilation for simple test done"
./compiler ./inFiles/complex.txt ./outFiles/complex.s
echo "Compilation for complex test done"
echo "All done"
exit 0
