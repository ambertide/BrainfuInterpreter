#include "parser.h"

void initParser(Parser *parser) {
    parser->input = (InputQueue *) malloc(sizeof(InputQueue));
    initInputQueue(parser->input);
    parser->tokenCount = 0;
}

void freeParser(Parser *parser) {
    free(parser->input);
}

inline Token getToken(char character) {
    switch (character) {
        case '>':
            return TOKEN_RIGHT;
        case '<':
            return TOKEN_LEFT;
        case '+':
            return TOKEN_INCREMENT;
        case '-':
            return TOKEN_DECREMENT;
        case '.':
            return TOKEN_OUTPUT;
        case ',':
            return TOKEN_INPUT;
        case '[':
            return TOKEN_FORWARD;
        case ']':
            return TOKEN_BACKWARD;
        case '!':
            return TOKEN_INPUT_SEPARATOR;
        default:
            return TOKEN_UNKNOWN;
    }
}

Token* parse(Parser *parser, const char* sourceCode, size_t length) {
    Token* tokens = (Token*) malloc(length * sizeof(Token));
    int tokenCount = 0;
    char *c = (char *) sourceCode;
    Token currentToken;
    int parsedCharacters = 0;
    while (*c != '\0') {
        currentToken = getToken(*c);
        c++;
        if (currentToken == TOKEN_UNKNOWN) {
            parsedCharacters++;
            continue; // Ignore unknown characters.
        } else if(currentToken == TOKEN_INPUT_SEPARATOR) {
            char copyBuffer[parsedCharacters + 1];
            memcpy(copyBuffer, sourceCode, parsedCharacters); // Copy the characters up until \0.
            copyBuffer[parsedCharacters] = '\0'; // Set this to terminate string properly.
            enqueueInput(parser->input, copyBuffer, parsedCharacters + 1); // Enqueue the input.
            tokenCount = 0; // Since those that came before were not real tokens, we start over.
        } else {
            *(tokens + tokenCount) = currentToken; // Otherwise attempt the token.
            tokenCount++;
        }
        parsedCharacters++;
    }
    tokens = realloc(tokens, (tokenCount) * sizeof(Token));
    parser->tokenCount = tokenCount;
    return tokens;
}