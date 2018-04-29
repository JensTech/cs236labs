// Robert Williams CS 236
// Run this command to compile: g++ -g -Wall -std=c++11 *.cpp -o lab1

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include "scanner.h"
#include "token.h"
using namespace std;

int main(int argc, char* argv[]) {
	if (argc > 0) {
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
	  // print results
	  for (unsigned int i = 0; i < tokens.size(); i++) {
	    cout << tokens[i]->toString() << endl;
	  }
	  cout << "Total Tokens = " << tokens.size() << endl;
	}
  return 0;
}
