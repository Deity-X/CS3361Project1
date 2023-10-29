#ifndef FRONT_H
#define FRONT_H

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define NEWLINE 2
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 27
#define LESSER_OP 28
#define GREATER_OP 29
#define EQUAL_OP 30
#define NEQUAL_OP 31
#define LEQUAL_OP 32
#define GEQUAL_OP 33
#define SEMICOLON 34
#define INC_OP 35
#define DEC_OP 36
#define KEY_READ 37
#define KEY_WRITE 38
#define KEY_WHILE 39
#define KEY_DO 40
#define KEY_OD 41

int lex();

#endif
