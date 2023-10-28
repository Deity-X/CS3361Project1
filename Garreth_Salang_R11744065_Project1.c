/* Name: Garreth Salang
   R#:R11744065
   Project 1 */
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define LESSER_OP 21
#define GREATER_OP 22
#define EQUAL_OP 23
#define NEQUAL_OP 24
#define LEQUAL_OP 25
#define GEQUAL_OP 26
#define SEMICOLON 27
#define ADD_OP 28
#define SUB_OP 29
#define MULT_OP 30
#define DIV_OP 31
#define INC_OP 32
#define DEC_OP 33
#define LEFT_PAREN 34
#define RIGHT_PAREN 35 
#define LEFT_CBRACE 36
#define RIGHT_CBRACE 37
#define KEY_READ 38
#define KEY_WRITE 39
#define KEY_WHILE 40
#define KEY_DO 41

int lex();
char tokenClass [100];
static void keyTerms();

/* Functions for BNF Grammar */
void stmt();
void character();
void expr();
void term();
void factor();
void op();
void variable();
void number();

/* Global Variable */
int nextToken;

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

/*****************************************************/
//lookup Fnction
static int lookup(char ch)
{
  //switch statement for lexeme and token
  switch(ch)
    {
      case '=':
        addChar();
        nextToken = ASSIGN_OP;
        strcpy(tokenClass, "ASSIGN_OP");
        //lookup for '=='
        if (nextChar == '=') 
        {
          addChar();
          nextToken = EQUAL_OP;
          strcpy(tokenClass, "EQUAL_OP");
          getChar();
        }
        break;
      
      case '<':
        addChar();
        nextToken = LESSER_OP;
        strcpy(tokenClass, "LESSER_OP");
        getChar();
        //lookup for '<='
        if (nextChar == '=') 
        {
          addChar();
          nextToken = LEQUAL_OP;
          strcpy(tokenClass, "LEQUAL_OP");
          getChar();
        }
        break;
      
      case '>':
        addChar();
        nextToken = GREATER_OP;
        strcpy(tokenClass, "GREATER_OP");
        getChar();
        //lookup for '>='
        if (nextChar == '=') 
        {
          addChar();
          nextToken = GEQUAL_OP;
          strcpy(tokenClass, "GEQUAL_OP");
          getChar();
        }
        break;
      
      case '!':
        addChar();
        nextToken = UNKNOWN;
        strcpy(tokenClass, "UNKNOWN");
        getChar();
        //lookup for '!='
        if (nextChar == '=') 
        {
          addChar();
          nextToken = NEQUAL_OP;
          strcpy(tokenClass, "NEQUAL_OP");
          getChar();
        }
        break;
      
      case ';':
        addChar();
        nextToken = SEMICOLON;
        break;
      
      case '+':
        addChar();
        nextToken = ADD_OP;
        strcpy(tokenClass, "ADD_OP");
        getChar();
        //lookup for '++'
        if (nextChar == '+') 
        {
          addChar();
          nextToken = INC_OP;
          strcpy(tokenClass, "INC_OP");
          getChar();
        }
        break;
      
      case '-':
        addChar();
        nextToken = SUB_OP;
        strcpy(tokenClass, "SUB_OP");
        getChar();
        //lookup for '--'
        if (nextChar == '-') 
        {
          addChar();
          nextToken = DEC_OP;
          strcpy(tokenClass, "DEC_OP");
          getChar();
        }
        break;
      
      case '*':
        addChar();
        nextToken = MULT_OP;
        break;
      
      case '/':
        addChar();
        nextToken = DIV_OP;
        break;
      
      case '(':
        addChar();
        nextToken = LEFT_PAREN;
        break;
      
      case ')':
        addChar();
        nextToken = RIGHT_PAREN;
        break;
      
      case '{':
        addChar();
        nextToken = LEFT_CBRACE;
        break;
      
      case '}':
        addChar();
        nextToken = RIGHT_CBRACE;
        break;
      
      default:
        addChar();
        nextToken = UNKNOWN;
        break;
    }
  return nextToken;
}

/*****************************************************/
//addChar function
static void addChar()
{
  if (lexLen <= 98)
  {
    lexeme[lexLen++] = nextChar;
    lexeme[lexLen] = 0;
  }
  else
  {
    printf("Error - lexeme is too long \n");
  }
}

/*****************************************************/
//getChar function
static void getChar()
{
  if ((nextChar = getc(in_fp) != EOF))
  {
    if (isalpha(nextChar))
    {
      charClass = LETTER;
    }
    else if (isdigit(nextChar))
    {
      charClass = DIGIT;
    }
    else
    {
      charClass = UNKNOWN;
    }
  }
  else
  {
    charClass = EOF;
  }
}

/*****************************************************/
//getNonBlank function
static void getNonBlank()
{
  while (isspace(nextChar)) getChar();
}

/*****************************************************/
//lex function
int lex()
{
  lexLen = 0;
  getNonBlank();

  switch (charClass)
  {
    case LETTER:
      addChar();
      getChar();
      while (charClass == LETTER || charClass == DIGIT)
      {
        addChar();
        getChar();
      }
      nextToken = IDENT;
      break;

    case DIGIT:
      addChar();
      getChar();
      while (charClass == DIGIT)
      {
        addChar();
        getChar();
      }
      nextToken = IDENT;
      break;

    case UNKNOWN:
      lookup(nextChar);
      getChar();
      break;

    case EOF:
      nextToken = EOF;
      lexeme[0] = 'E';
      lexeme[1] = 'O';
      lexeme[2] = 'F';
      lexeme[3] = 0;
      break;
  }
  return nextToken;
}
/*****************************************************/
/* Function for assigning read, write, while, do */
static void keyTerms()
{
  char keyword[10];
  int keywordLength = 0;

  while (isalpha(nextChar)) 
  {
    keyword[keywordLength++] = nextChar;
    addChar();
    getChar();
  }

  if (strcmp(keyword, "read") == 0)
  {
    nextToken = KEY_READ;
  }
  else if (strcmp(keyword, "write") == 0)
  {
    nextToken = KEY_WRITE;
  }
  else if (strcmp(keyword, "while") == 0)
  {
    nextToken = KEY_WHILE;
  }
  else if (strcmp(keyword, "do") == 0)
  {
    nextToken = KEY_DO;
  } 
  else
  {
    nextToken = IDENT;
  }
}

/*****************************************************/
/*The following functions are a part of parser.c*/
/*****************************************************/
/* Function for S ::= V=E | read(V) | write(V) | do { S } while (C) | S;S */
void stmt()
{
  //read(V)
  if (nextToken == KEY_READ||nextToken == KEY_WRITE)
  {
    lex();
      if (nextToken == LEFT_PAREN)
      {
          lex();
          variable();

        if (nextToken == RIGHT_PAREN)
        {
          lex();
        }
      }
  }
  //write(V)
  else if (nextToken == KEY_WRITE)
  {
    lex();
    if (nextToken == LEFT_PAREN)
    {
        lex();
        variable();

      if (nextToken == RIGHT_PAREN)
      {
        lex();
      }
    }
  }
  //do { S } while (C)
  else if (nextToken == KEY_DO)
  {
      lex();
      if (nextToken == LEFT_CBRACE)
      {
        lex();
        stmt();
      }
        if (nextToken == RIGHT_CBRACE)
        {
          lex();
        }
          if (nextToken == KEY_WHILE)
          {
            lex();
            stmt();
          }
            if (nextToken == LEFT_PAREN)
            {
              lex();
              character();
            }
             if (nextToken == RIGHT_PAREN)
              {
                lex();
              }
  }
  //S;S
  else if (nextToken == SEMICOLON)
  {
    lex();
    stmt();
  }
}

/*****************************************************/
/* Function for C ::= E < E | E > E | E == E | E != E | E <= E | E >= E */
void character() 
{
  /* Parse the first term */
  expr();

  while (nextToken == LESSER_OP || nextToken == GREATER_OP|| nextToken == EQUAL_OP|| nextToken == NEQUAL_OP|| nextToken == LEQUAL_OP|| nextToken == GEQUAL_OP) 
  {
    lex();
    expr();
  }
}

/*****************************************************/
/* Function for E ::= T | E + T | E - T */
void expr()
{
  /*Parse the first term*/
  term();

  while (nextToken == ADD_OP || nextToken == SUB_OP)
    {
      lex();
      term();
    }
}

/*****************************************************/
/* Function for T ::= F | T * F | T / F*/
void term()
{
  /* Parse the first factor */
  factor();
  /* As long as the next token is * or /, get the
  next token and parse the next factor */
  while (nextToken == MULT_OP || nextToken == DIV_OP)
  {
    lex();
    factor();
  }
}

/*****************************************************/
/* Function for F ::= (E) | O | N | V */
void factor()
{
  //(E)
  if (nextToken == LEFT_PAREN || nextToken == RIGHT_PAREN) 
  {
    if (nextToken == LEFT_PAREN) 
    {
      lex();
    }
    else if (RIGHT_PAREN)
    {
      lex();
    }
  } 
  //O
  else if (nextToken == INC_OP || nextToken == DEC_OP)
  {
    op();
  } 
  //N
  else if (nextToken == INT_LIT)
  {
    number();
  } 
  //V
  else if (nextToken == IDENT)
  {
    variable();
  }
}

/*****************************************************/
/* Function for O ::= V++ | V-- */
void op()
{
  lex();
}

/*****************************************************/
/* Function for V ::= a | b | … | y | z | aV | bV | … | yV | zV */
void variable()
{
  lex();
}

/*****************************************************/
/* Function for N ::= 0 | 1 | … | 8 | 9 | 0N | 1N | … | 8N | 9N */
void number()
{
  lex();
}
  
/******************************************************/
/* main driver */
int main() 
{
  printf("DCooke Analyzer :: R11744065 \n");
  /* Open the input data file and process its contents */
  if ((in_fp = fopen("front.in", "r")) == NULL) 
  {
    printf("ERROR - cannot open front.in \n");
  } 
  else 
  {
    getChar();
    do 
    {
      lex();
      stmt();
    } 
    while (nextToken != EOF);
  }
  return 0;
}