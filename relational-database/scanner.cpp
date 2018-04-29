// Robert Williams CS 236

#include "scanner.h"
#include <ctype.h>
#include <vector>
using namespace std;

Scanner::Scanner(istream* in) {
  this->in = in;
}

Scanner::~Scanner() {
}

vector<Token*> Scanner::lexicalAnalysis() {
  char c;
  vector<Token*> tokens;
  this->line = 1;
  while (true) {
    // read in new character
    c = this->in->get();
    // break at end of file
    if (this->in->eof()) {
      tokens.push_back(new Token(END_OF_FILE, "", this->line));
      break;
    }
    // ignore whitespace, but increment line counter if newline
    if (isspace(c)) {
      if (c == '\n') this->line++;
      continue;
    }

    // Finite State Machine (sort of)
    switch (c) {
      case ',':
      tokens.push_back(new Token(COMMA, ",", this->line));
      break;

      case '.':
      tokens.push_back(new Token(PERIOD, ".", this->line));
      break;

      case '?':
      tokens.push_back(new Token(Q_MARK, "?", this->line));
      break;

      case '(':
      tokens.push_back(new Token(LEFT_PAREN, "(", this->line));
      break;

      case ')':
      tokens.push_back(new Token(RIGHT_PAREN, ")", this->line));
      break;

      case ':':
      if (this->in->peek() == '-') {
        this->in->get();
        tokens.push_back(new Token(COLON_DASH, ":-", this->line));
      } else {
        tokens.push_back(new Token(COLON, ":", this->line));
      }
      break;

      case '*':
      tokens.push_back(new Token(MULTIPLY, "*", this->line));
      break;

      case '+':
      tokens.push_back(new Token(ADD, "+", this->line));
      break;

      case '\'':
      tokens.push_back(this->consume_string());
      break;

      case '#':
      if (this->in->peek() == '|') {
        this->in->get();
        // remove comments
        //tokens.push_back(this->consume_block_comment());
        delete this->consume_block_comment();
      } else {
      	// remove comments
        //tokens.push_back(this->consume_line_comment());
        delete this->consume_line_comment();
      }
      break;

      default:
      // make sure first character is alphanumeric
      if (!isalpha(c)) {
        string extracted = "";
        extracted += c;
        tokens.push_back(new Token(UNDEFINED, extracted, this->line));
      } else {
        this->in->putback(c);
        tokens.push_back(this->consume_alpha_numeric());
      }
    }
  }

  return tokens;
}

Token* Scanner::consume_alpha_numeric() {
  string extracted = "";
  while (isalpha(this->in->peek()) || isalnum(this->in->peek())) {
    extracted += this->in->get();
  }

  if (this->keyword.find(extracted) != this->keyword.end()) {
    return new Token(this->keyword[extracted], extracted, this->line);
  } else {
    return new Token(ID, extracted, this->line);
  }
}

Token* Scanner::consume_line_comment() {
  string extracted = "#";
  int start_line = this->line;
  char c;
  while (true) {
    c = this->in->get();
    // comment ends on end of file or new line
    if (this->in->eof()) {
      break;
    }
    if (c == '\n') {
      this->line++;
      break;
    }
    // append character to comment
    extracted += c;
  }
  return new Token(COMMENT, extracted, start_line);
}

Token* Scanner::consume_block_comment() {
  string extracted = "#|";
  int start_line = this->line;
  char c;
  while (true) {
    c = this->in->get();
    // return undefined on end of file
    if (this->in->eof()) {
      return new Token(UNDEFINED, extracted, start_line);
    }
    // end comment on |#
    if (c == '|' && this->in->peek() == '#') {
      this->in->get();
      extracted += "|#";
      return new Token(COMMENT, extracted, start_line);
    }
    // account for new lines
    if (c == '\n') this->line++;
    // append character to comment
    extracted += c;
  }
}

Token* Scanner::consume_string() {
  string extracted = "'";
  int start_line = this->line;
  char c;
  while (true) {
    c = this->in->get();
    // if there is a end of file,
    if (this->in->eof()) {
      return new Token(UNDEFINED, extracted, start_line);
    }

    if (c == '\'') {
      if (this->in->peek() == '\'') {
        // if there are two quotes, continue the string
        extracted += c;
        c = this->in->get();
      } else {
        // if there is one quote, end the string
        extracted += '\'';
        return new Token(STRING, extracted, start_line);
      }
    }

    // increment the line counter if you run across a new line
    if (c == '\n') this->line++;

    // append the character to the extracted string and continue
    extracted += c;
  }
}
