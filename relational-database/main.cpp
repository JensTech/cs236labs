// Robert Williams CS 236
// Run this command to compile: g++ -g -Wall -std=c++11 *.cpp -o lab1

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include "scanner.h"
#include "token.h"
#include "parser.h"
#include "datalogProgram.h"
#include "interpreter.h"
using namespace std;

int main(int argc, char* argv[]) {
	if (argc == 0) return 0;

	// get the filename from the args
	string filename = string(argv[1]);
	ifstream file;
	file.open(filename);
	// check if file opened successfully
	if (!file.good()) {
		cout << "File '" << filename << "' could not be opened." << endl;
		return 0;
	}
	// pass ifstream to scanner
	Scanner scanner = Scanner(&file);
	// run lexer to get a vector of tokens
  	vector<Token*> tokens = scanner.lexicalAnalysis();
  	
	// pass the tokens to the parser
	Parser parser = Parser(tokens);
	// now run the parser
	DatalogProgram* parsedProgram = parser.datalogParsing();

	// pass the programe to the interpreter
	Interpreter interpreter = Interpreter(parsedProgram);
	// build the database
	interpreter.buildDatabase();
	cout << interpreter.runQueries();

	// clean up
	delete parsedProgram;

	return 0;
}
