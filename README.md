# ARMCompiler
creation of ARM instruccions based in SECD instructions

## Previous installation

The program use sexpr library, this means that in order to work, it's neccessary to have installed this library. To do that please go to the main mage of the proyect here: https://github.com/mjsottile/sfsexp

For the reset, you will only need c++ installed with g++ compiler and standard libraries.

## compilation

To compile the main program, just run

```angular2
g++ -o compiler compiler .cpp -lsexp
```

The use of the compiler follow:

```angular2
./compiler archivo.txt archivo.s
```

## test execution

To use the test, you just need to run the bash file:

```angular2
./run.sh
```

and then, go to the ./outFiles folder and run:

```angular2
./test.sh
```

this need to be excecuted in an ARM machine.

## Context

This project was made as homework for the course 'Architecture of computers' of University of chile.
There is a small report document created for the course(in spanish) that can be helpful to 
understand the program.