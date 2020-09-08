#ifndef brainfuc_parser_h
#define brainfuc_parser_h
#include "common.h"
#include "input.h"

typedef enum {
    TOKEN_INCREMENT,
    TOKEN_DECREMENT,
    TOKEN_RIGHT,
    TOKEN_LEFT,
    TOKEN_OUTPUT,
    TOKEN_INPUT,
    TOKEN_FORWARD,
    TOKEN_BACKWARD,
    TOKEN_INPUT_SEPARATOR,
    TOKEN_UNKNOWN
} Token;

typedef struct {
    size_t tokenCount;
    InputQueue *input;
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
extern Token getToken(char character);

/**
 * Free a parser.
 */
void freeParser(Parser* parser);

Token* parse(Parser* parser, const char* sourceCode, size_t length);


#endif