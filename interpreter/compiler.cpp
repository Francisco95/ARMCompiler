#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>
#include <string.h>
#include <vector>
#include "sexp.h"
// #include "sexp_ops.h"

using namespace std;

// in/out files
FILE *in, *out;
// [r4 - r11]. Preserved, stack before using, 
// restore before returing.
vector<int> preserved(8, 0);
// function string stream 
ostringstream fun_str (ostringstream::ate);
int P = 0; // integer counter for labels.
int BUF_SIZE = 4096; // buf size

// function heads
void int_const(elt *elt, ostringstream& father);
void add(ostringstream& father);
void sub(ostringstream& father);
void apply(ostringstream& father);
void if0(elt *elt, ostringstream& father);
void fun(elt *elt, ostringstream& father);
void go_to(elt *elt, char val[], ostringstream& father);
void loop(elt *elt, ostringstream& father);

/*
 * get the next preserved register that is free
 */
int next_preserved(){
	for (int i = 0; i < preserved.size(); ++i){
		if (!preserved[i]){
			preserved[i] = 1;
			return i+4;
		}
	}
	fprintf(stderr, "All preserved register are used\n");
	exit(1);
}

/*
 * set all preserved register as available
 */
void all_preserved_available(){
	for (int i = 0; i < preserved.size(); ++i)
	{
		preserved[i] = 0;
	}
}

/*
 * set free a preserved register
 */
void release_preserved(int ri){ // release the current reg.
	preserved[ri-4] = 0;
}

/*
 * create ARM assemlby code for SECD instruction INT_CONST
 */
void int_const(elt *elt, ostringstream& father){
	// get a free register
	int p = next_preserved();
	// mov the integer value to a register
	father << "\tmov r" << p << " ,#" << elt->next->val << "\n";
	// and store it in the stack
	father << "\tstmfd r13!, {r" << p << "}\n";
	// then release the register
	release_preserved(p);
}

/*
 * create ARM assembly code for SECD instruction ADD/SUB 
 */
void op(ostringstream& father, string ops){
	// get two free registers
	int p1 = next_preserved();
	int p2 = next_preserved();
	// load them
	father << "\tldmfd r13!, {r" << p1 << "}\n";
	father << "\tldmfd r13!, {r" << p2 << "}\n";
	// and operate them
	father << "\t" << ops.c_str() << " r" << p2 << ", r" << p1 << ", r" << p2 << "\n";
	// store the result in the stack
	father << "\tstmfd r13!, {r" << p2 << "}\n";
	// and set free the registers
	release_preserved(p1);
	release_preserved(p2);
}

/*
 * create ARM assembly code for SECD instruction APPLY
 */
void apply(ostringstream& father){
	// load last value in register as parameter to a funtion
	// (in register r0)
	father << "\tldmfd r13!, {r0}\n";
	// get a free preserved register
	int p1 = next_preserved();
	// and load in it the label to the function
	father << "\tldmfd r13!, {r" << p1 << "}\n";
	// jump to that function
	father << "\tblx r" << p1 << "\n";
	// set free the preserved register
	release_preserved(p1);
	// and store in the stack the returned value by the function
	father << "\tstmfd r13!, {r0}\n";
}

/*
 * create ARM assembly code for SECD instruction IF0
 */
void if0(elt *elt, ostringstream& father){
	P++; // new label
	// copy in local variable to avoid problems with nested functions
	int tmp = P;
	// get a free register.
	int p1 = next_preserved();
	// load the last value of the stack
	father << "\tldmfd r13!, {r" << p1 << "}\n";
	// compare this value to 0
	father << "\tcmp r" << p1 << ", #0\n";
	// then we can set free the register
	release_preserved(p1);
	// and in the case that value != 0, jump to label
	father << "\tblne if0" << tmp << "\n";
	// else(value == 0), access to the respective instructions
	loop(elt->next->list, father);
	// and jump to the end of the if statement
	father << "\tbl if0" << tmp << "_end\n";
	// create the label for the if statement when value!=0
	father << "if0" << tmp << ":\n";
	// access to the respective instructions
	loop(elt->next->next->list, father);
	// create the label for the end of the if statement.
	father << "if0" << tmp << "_end:\n";
}

/*
 * create ARM assembly code for SECD instruction RETURN
 */
void fun_return(ostringstream& father){
	// load last value in stack to return as parameter 
	// r0 (return parameter)
	father << "\tldmfd r13!, {r0}\n"; 
	// and restore all registers
	father << "\tldmfd r13!, {r4, r5, r6, r7, r8, r9, r10, r11, pc}\n";
}

/*
 * create ARM assembly code for SECD instruction FUN
 */
void fun(elt *elt, ostringstream& father){
	// create a string stream to store the instruction 
	// to compile for this function
	ostringstream actual(ostringstream::ate);
	// get a free register
	int p1 = next_preserved();
	P++; // new segment
	// load in register the label of the function
	father << "\tldr r" << p1 << ", =fun" << P << "\n";
	// store the register value in the stack
	father << "\tstmfd r13!, {r" << p1 << "}\n";
	// and set it free
	release_preserved(p1);

	// then we access to the instruction that are going to
	// generate the code inside the function
	elt = elt->next;
	// create the label
	actual << "fun" << P << ":\n";
	// start the stack in the function preservin the registers r4-r11
	actual << "\tstmfd r13!, {r4, r5, r6, r7, r8, r9, r10, r11, lr}\n";
	// put in stack the function parameter (r0)
	actual << "\tstmfd r13!, {r0}\n";
	// then keep the actual preserved register status
	vector<int> tmp = preserved;
	// and set all of them as free
	all_preserved_available();
	// start the definition of the function code
	loop(elt, actual);
	// restore preserved register values
	preserved = tmp; 
	// and return the function
	fun_return(actual);
	// save all of this code in fun string stream
	// this string stream has the ARM assembly code
	// for all functions.
	fun_str << actual.str();
}

/*
 * function that check the SECD instruction name 
 * and decide what function excecute.
 */
void go_to(elt *elt, char val[], ostringstream& father){
	if (strncmp(val, "INT_CONST", 10) == 0) int_const(elt->list, father);
	else if (strncmp(val, "IF0", 4) == 0) if0(elt->list, father);
	else if (strncmp(val, "FUN", 4) == 0) fun(elt->list, father);
	else if (strncmp(val, "ADD", 4) == 0) op(father, "add");
	else if (strncmp(val, "SUB", 4) == 0) op(father, "sub");
	else if (strncmp(val, "APPLY", 4) == 0) apply(father);
}

/*
 * function  that generate a loop over all sub-instructions inside a main instruction,
 * for example, this loop allow to get all the instructions that are going to be in
 * 'main' or all instructions that are goig to be in any function 'funN'.
 */
void loop(elt *elt, ostringstream& father){
	while(true){
		// if there is no more instructions, end
		if (elt == NULL) break; 
		// if there is no more sub-instructions, end
		if (elt->list == NULL) break;
		else{
			// if the instruction has no value (instruction name), end
			// this can happens with empty parenthesis.
			if (elt->list->val == NULL) break;
			// else, go to the respective function that read the instruction.
			else go_to(elt, elt->list->val, father);
		}
		// and repeat with the next instruction.
		elt = elt->next;
	}
}

/*
 * create the ARM assembly instructions that are going to be 
 * at the top of the file, and thus, at the top of the function
 * string stream.
 */
void top_fun(ostringstream& father){
	// declare the global label
	father << "\t.globl main\n";
	// declare use of printf function
	father << "\t.extern printf\n";
	// declare the text part
	father << "\t.text\n";
	// declare an output string 
	father << "output_str:\n";
	// this string shows an integer (the result of the program)
	father << "\t.ascii \"ans: %d\\n\"\n";
	// which is alinged.
	father << "\t.align 4\n";
}

/*
 * create the ARM assembly instructions that are going to be
 * at the bottom of the file, and this, at the bottom of the 
 * main string stream
 */
void bottom_main(ostringstream& father){
	// load result number of the program in r1
	father << "\tldmfd r13!, {r1}\n"; 
	// define r0 as the string with the format to print
	father << "\tldr r0, =output_str\n"; 
	// and call printf function
	father << "\tbl printf\n";
	// then set r0 as 0 (just in case)
	father << "\tmov r0, #0\n";
	// and end the stack
	father << "\tldmfd sp!, {ip, pc}\n";
}

/*
 * create the ARM assemble instructions that are going to be
 * at the top of main, here we declare the main label and 
 * initiate the stack
 */
void top_main(ostringstream& father){
	// main label
	father << "main:\n";
	// init stack
	father << "\tstmfd r13!, {ip, lr}\n";
}

/*
 * allow to read files from different folders and write 
 * in a different folder too
 */
void read_files(string file_in, string file_out){
	// keep the initial dir path
	char init_path[100];
	char *path = getcwd(init_path, 100);

	size_t pos_in = file_in.find_last_of("/");
	size_t pos_out = file_out.find_last_of("/");

	// by found the last / of the path, we can 
	// extract the substring with the path dir
	string dir_in = file_in.substr(0, pos_in);
	string dir_out = file_out.substr(0,pos_out);

	int rc = chdir(dir_in.c_str()); //change to dir path of in file
	if (rc == -1){ // if cannot change, throws error
		fprintf(stderr, 
			"cannot access to folder '%s' from input file\n", 
			dir_in.c_str());
		exit(1);
	}
	// and if it changes, we open the file by getting the 
	// substring posterior to the last '/'
	in = fopen(file_in.substr(pos_in+1, file_in.size() - pos_in).c_str(), "r+");

	// then return to the initial path
	rc = chdir(init_path);

	// and go to the dir path of output file
	rc = chdir(dir_out.c_str());
	if (rc == -1){ // if cannot go, throws error
		fprintf(stderr, 
			"cannot access to folder '%s' from output file\n", 
			dir_out.c_str());
		exit(1);
	}
	// and if it changes, open/create the output file by 
	// getting the substring posterior to the last '/'
	out = fopen(file_out.substr(pos_out+1, file_out.size() - pos_out).c_str(), "w+");

	// finally return to the initial path
	rc = chdir(init_path);
}

/*
 * main function fo create the compiler, it receives the relative 
 * paths to the input filder and output filder 
 * (including folders name).
 *
 * example:  ./compile ./folder1/infile.txt ./folder2/outfile.txt
 */
int main(int argc, char *argv[]){
	// create string stream for main function.
	ostringstream main (ostringstream::ate);
	// define top ARM assembly instructions.
	top_main(main);
	top_fun(fun_str);

	if (argc!=3){
		fprintf(stderr, 
			"use: %s <./folder/where/is/file_in> <./folder/where/is/file_out>\n", 
			argv[0]);
		exit(1);
	}

	// read file where are the SECD instrunctions and file 
	// were we write the ARM assembly code
	read_files(argv[1], argv[2]);
	// FILE *in = fopen(file_in_name.c_str(), "r+"); // open file with instructions
	// FILE *out = fopen(file_out_name.c_str(), "w+"); // open file to write code

	// variable to parse the instructions
	sexp_t *sx; 
	char buf[BUF_SIZE];
	while(fread(buf, 1, BUF_SIZE, in)){
		// parse instruction of BUF_SIZE bytes len
		sx = parse_sexp(buf, BUF_SIZE);
		// enter loop of instrunction that are going to be in main function
		loop(sx->list, main);
		// destroy the parse
		destroy_sexp(sx);
	}
	// ARM assembly code for the end of main function
	bottom_main(main);
	// and write to the file.
	fprintf(out, "%s\n", fun_str.str().c_str());
	fprintf(out, "%s\n", main.str().c_str());
	return  0;
}