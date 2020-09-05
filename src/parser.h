#ifndef brainfuc_parser_h
#define brainfuc_parser_h
#include "common.h"

typedef enum {
    TOKEN_INCREMENT,
    TOKEN_DECREMENT,
    TOKEN_RIGHT,
    TOKEN_LEFT,
    TOKEN_OUTPUT,
    TOKEN_INPUT,
    TOKEN_FORWARD,
    TOKEN_BACKWARD,
    TOKEN_UNKNOWN
} Token;

typedef struct {
    size_t tokenCount;
    Token tokenJmpTable[51];
} Parser;

/**
 * Initialise a parser by initialising
 * its tokenJmpTable which maps tokens
 * to character using ASCII values.
 */
void initParser(Parser* parser);

/**
 * Given a character translate it to
 * its token equivalent.
 */
Token* getToken(Parser* parser, const char* character);

/**
 * Free a parser.
 */
void freeParser(Parser* parser);

Token* parse(Parser* parser, const char* sourceCode, size_t length);


#endif