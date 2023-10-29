#include <stdio.h>
#include <ctype.h>
#include <string.h>

/*Data from front.h*/
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

/*Data from parser.h*/
void stmt();
void c();
void expr();
void term();
void factor();
void o();
void var();
void num();

extern int nextToken;

/* Global Variable */
int nextToken;
char tokenStr[20];

/* Local Variables */
static int charClass;
static char lexeme [100];
static char nextChar;
static int lexLen;
static FILE *in_fp;

/* Local Function declarations */
static void addChar();
static void getChar();
static void getNonBlank();
static void wordFinder();

/******************************************************/
/* main driver */
int main(int argc, char* argv[]) 
{
    
    printf("DCooke Analyzer :: R11677203\n\n");
    /* Open the input data file and process its contents */
    if ((in_fp = fopen(argv[1], "r")) == NULL) {
        printf("ERROR - cannot open file \n");
    } else {
        getChar();
        do {
            lex();
            stmt();
        } while (nextToken != EOF);
    }

    return 0;
}

/*****************************************************/
/* lookup - a function to lookup operators and parentheses and return the 
 * token */
static int lookup(char ch) {
  switch (ch) {
        case '(':
          addChar();
            nextToken = LEFT_PAREN;
            strcpy(tokenStr, "LEFT_PAREN");
            getChar();
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            strcpy(tokenStr, "RIGHT_PAREN");
            getChar();
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            strcpy(tokenStr, "ADD_OP");
            getChar();
            if(strcmp(&nextChar, "+") == 0){
              addChar();
                nextToken = INC_OP;
                strcpy(tokenStr, "INC_OP");
                getChar();
            }
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            strcpy(tokenStr, "SUB_OP");
            getChar();
            if(strcmp(&nextChar, "-") == 0){
              addChar();
                nextToken = DEC_OP;
                strcpy(tokenStr, "DEC_OP");
                getChar();
            }
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            strcpy(tokenStr, "MULT_OP");
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            strcpy(tokenStr, "DIV_OP");
            break;
        case '<':
            addChar();
            nextToken = LESSER_OP;
            strcpy(tokenStr, "LESSER_OP");
            getChar();
            if(strcmp(&nextChar, "=") == 0){
              addChar();
                nextToken = LEQUAL_OP;
                strcpy(tokenStr, "LEQUAL_OP");
                getChar();
            }
            break;
        case '>':
            addChar();
            nextToken = GREATER_OP;
            strcpy(tokenStr, "GREATER_OP");
            getChar();
            if(strcmp(&nextChar, "=") == 0){
              addChar();
                nextToken = GEQUAL_OP;
                strcpy(tokenStr, "GEQUAL_OP");
                getChar();
            }
            break;
        case ';':
            addChar();
            nextToken = SEMICOLON;
            strcpy(tokenStr, "SEMICOLON");
            getChar();
            break;
        case ':':
            addChar();
            getChar();
            if(strcmp(&nextChar, "=") == 0){
              addChar();  
              nextToken = ASSIGN_OP;
              strcpy(tokenStr, "ASSIGN_OP");
                getChar();
            }
            break;
        case '!':
            addChar();
            getChar();
            if(strcmp(&nextChar, "=") == 0){
                nextToken = NEQUAL_OP;
                strcpy(tokenStr, "NEQUAL_OP");
                getChar();
            }
            break;
        default:
            addChar();
            nextToken = UNKNOWN;
            strcpy(tokenStr, "UNKNOWN");
            getChar();
            break;
    }
    return nextToken;
}

/*****************************************************/
/* addChar - a function to add nextChar to lexeme */
static void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("Error - lexeme is too long \n");
    }
}

/*****************************************************/
/* getChar - a function to get the next character of input and determine its 
 * character class */
static void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else if (strcmp(&nextChar, "\n")==0)
            charClass = NEWLINE;
        else charClass = UNKNOWN;
    } else {
        charClass = EOF;
    }
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it returns a non-whitespace 
 * character */
static void getNonBlank() {
    while (isspace(nextChar)) getChar();
}

/*****************************************************/
/* wordFinder - a function to identify if the string of characters is a keyword
or an identifier */
static void wordFinder(){
    int i = 0;
    addChar();
    i++;
    getChar();
    while (charClass == LETTER) {
        addChar();
        getChar();
        i++;
    }
    if(strncmp(lexeme, "read", 4) == 0&&strlen(lexeme)==4){ 
      nextToken = KEY_READ;
        strcpy(tokenStr, "KEY_READ");
    }
    else if(strncmp(lexeme, "write",5) == 0&&strlen(lexeme)==5){
        nextToken = KEY_WRITE;
        strcpy(tokenStr, "KEY_WRITE");
    }
    else if(strncmp(lexeme, "while",5) == 0&&strlen(lexeme)==5){
        nextToken = KEY_WHILE;
        strcpy(tokenStr, "KEY_WHILE");
    }
    else if(strncmp(lexeme, "do", 2) == 0&&strlen(lexeme)==2){ 
      nextToken = KEY_DO;
        strcpy(tokenStr, "KEY_DO");
    }
    else if(strncmp(lexeme, "od",2) == 0){ 
      nextToken = KEY_OD;
        strcpy(tokenStr, "KEY_OD");
    }
    else{
        nextToken = IDENT;
        strcpy(tokenStr, "IDENT");
    }
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic expressions */
int lex() {
    lexLen = 0;
    getNonBlank();
    switch (charClass) {
        /* Parse identifiers and keywords*/
        case LETTER:
            wordFinder();
            break;

        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            strcpy(tokenStr, "INT_LIT");
            break;

       /* For newline character */
        case NEWLINE:
            getChar();

        /* Symbols */
        case UNKNOWN:
            lookup(nextChar);
            break;

        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    } /* End of switch */

    if(strncmp(lexeme,"EOF",3)!=0)
      printf("%s\t%s\n", lexeme, tokenStr);
    return nextToken;
} /* End of function lex */

/* stmt
 * Parses strings in the language generated by the rule:
 * S ::= V:=E|read(V)|write(V)|while (C) do S od|O|S;S
 */
void stmt() 
{
    /* Parse read or write */
    if(nextToken == KEY_READ||nextToken == KEY_WRITE){
      lex();
        if (nextToken == LEFT_PAREN){
            lex();
            var();
        
          if (nextToken == RIGHT_PAREN)
              lex();
        }
    }

    /*If starts with while*/
    else if(nextToken == KEY_WHILE){
        lex();
        if (nextToken == LEFT_PAREN){
            lex();
            c();}
            if (nextToken == RIGHT_PAREN){
                lex();}
                if (nextToken == KEY_DO){
                    lex();
                    stmt();}
                    if (nextToken == KEY_OD){
                        lex();}
    }

/*Differ between O and V:=E*/
    else if(nextToken==IDENT){
        var();
        if(nextToken==ASSIGN_OP){ //FOR V:=E
            lex();
            expr();
        }
        else if(nextToken==INC_OP||nextToken==DEC_OP){ //FINISH OFF O WITHOUT CALLING FUNCTION
            lex();
        }
    }
    else if(nextToken==INC_OP||nextToken==DEC_OP){ //STARTS O IF SYMBOL STARTS IT
        lex();
        o();
    }

    if(nextToken==SEMICOLON){
        lex();
        stmt();
    }
} /* End of function stmt */

/* c
 * Parses strings in the language generated by the rule:
 * C ::= E<E|E>E|E=E|E!=E|E<=E|E>=E
 */

void c() 
{
  /* Parse the first term */
    expr();

    /* As long as the next token is in the language, get
    the next token and parse the next term */
    while (nextToken == LESSER_OP || nextToken == GREATER_OP|| nextToken == EQUAL_OP|| nextToken == NEQUAL_OP|| nextToken == LEQUAL_OP|| nextToken == GEQUAL_OP) {
        lex();
        expr();
    }
} /* End of function c */

/* expr
 * Parses strings in the language generated by the rule:
 * E ::= T|E+T|E-T
 */
void expr() 
{
    /* Parse the first term */
 term();
/* As long as the next token is + or -, get
 the next token and parse the next term */
 while (nextToken == ADD_OP || nextToken == SUB_OP) {
 lex();
 term();
 }
} /* End of function expr */

/* term
 * Parses strings in the language generated by the rule:
 * T ::= F|T*F|T/F
 */
void term() 
{
    /* Parse the first factor */
    factor();
    /* As long as the next token is * or /, get the
    next token and parse the next factor */
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        lex();
        factor();
    }
} /* End of function term */

/* factor
 * Parses strings in the language generated by the rule:
 * F ::= (E)|O|N|V
 * */
void factor() {

    /* CHECK FOR N */
    if (nextToken == INT_LIT)
        num();

    /* Differentiate between O and V */

    else if(nextToken==IDENT){
        var();
        if(nextToken==INC_OP||nextToken==DEC_OP){ //FINISH OFF O WITHOUT CALLING FUNCTION
            lex();
        }
    }
    else if(nextToken==INC_OP||nextToken==DEC_OP){ //STARTS O IF SYMBOL IS AT BEGINNING
        lex();
        o();
    }
    
    else {
        if (nextToken == LEFT_PAREN) {
            lex();
            expr();
            if (nextToken == RIGHT_PAREN)
                lex();
        } /* End of if (nextToken == ... */
    } /* End of else */
} /* End of function factor */

/* o
 * Parses strings in the language generated by the rule:
 * O ::= V++|V--|++V|--V
 */

void o() {
    var();
} /* End of function o */

/* var
 * Parses strings in the language generated by the rule:
 * V ::= a|b|...|y|z|aV|bV|...|yV|zV
 */

void var() {
   lex();
} /* End of function var */

/* num
 * Parses strings in the language generated by the rule:
 * N ::= 0|1|...|8|9|0N|1N|...|8N|9N
 */

void num() {
    lex();
} /* End of function num */
