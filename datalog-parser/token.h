// Robert Williams CS 236

#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <stdio.h>
#include <string>
#include <map>
using namespace std;

// Different types of tokens
enum tokenType {COMMA, PERIOD, Q_MARK, LEFT_PAREN, RIGHT_PAREN, COLON, COLON_DASH, MULTIPLY, ADD, SCHEMES, FACTS, RULES, QUERIES, ID, STRING, COMMENT, WHITESPACE, UNDEFINED, END_OF_FILE, EXPRESSION};

class Token {
public:
  Token(tokenType type, string extracted, int line);
  ~Token();
  // print out a string representation of the entire token
  string toString();
  string getExtracted();
  // access private variables publicly
  tokenType getType();
  string getText();
  int getLine();
  // convert token types to string
  map<tokenType, string> tokenString {{COMMA, "COMMA"}, {PERIOD, "PERIOD"}, {Q_MARK, "Q_MARK"}, {LEFT_PAREN, "LEFT_PAREN"}, {RIGHT_PAREN, "RIGHT_PAREN"}, {COLON, "COLON"}, {COLON_DASH, "COLON_DASH"}, {MULTIPLY, "MULTIPLY"}, {ADD, "ADD"}, {SCHEMES, "SCHEMES"}, {FACTS, "FACTS"}, {RULES, "RULES"}, {QUERIES, "QUERIES"}, {ID, "ID"}, {STRING, "STRING"}, {COMMENT, "COMMENT"}, {WHITESPACE, "WHITESPACE"}, {UNDEFINED, "UNDEFINED"}, {END_OF_FILE, "EOF"}};

private:
  tokenType type;
  string extracted;
  int line;
};

#endif /* TOKEN_H */
