#include <iostream>
#include <stdio.h>
#include <fstream>
#include "scanner.h"
using namespace std;

int main() {
  ifstream file;
  file.open("test.txt");
  Scanner scanner = Scanner(&file);
  scanner.lexicalAnalysis();
  return 0;
}
