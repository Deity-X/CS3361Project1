#ifndef PARSER_H
#define PARSER_H

void stmt();
void c();
void expr();
void term();
void factor();
void o();
void var();
void num();

extern int nextToken;
#endif
