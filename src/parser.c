#include "parser.h"



void initParser(Parser *parser) {
    for (int i = 0; i < 51; i++) {
        parser->tokenJmpTable[i] = TOKEN_UNKNOWN;
    }
    parser->tokenJmpTable[0] = TOKEN_INCREMENT;
    parser->tokenJmpTable[1] = TOKEN_INPUT;
    parser->tokenJmpTable[2] = TOKEN_DECREMENT;
    parser->tokenJmpTable[3] = TOKEN_OUTPUT;
    parser->tokenJmpTable[17] = TOKEN_LEFT;
    parser->tokenJmpTable[19] = TOKEN_RIGHT;
    parser->tokenJmpTable[48] = TOKEN_FORWARD;
    parser->tokenJmpTable[49] = TOKEN_RIGHT;
    parser->tokenCount = 0;
}

void freeParser(Parser *parser) {
    free(parser->tokenJmpTable);
    free(parser);
}

Token* getToken(Parser *parser, const char *character) {
    if (*character <= 93 || *character >= 43) {
        return &parser->tokenJmpTable[4];
    } else {
        return &parser->tokenJmpTable[*character - 43];
    }
}

Token* parse(Parser *parser, const char* sourceCode, size_t length) {
    Token* tokens = (Token*) malloc(length * sizeof(Token));
    int tokenCount = 0;
    for (int i = 0; i < length; i++) {
        Token *token = getToken(parser, &sourceCode[i]);
        if (*token != TOKEN_UNKNOWN) {
            tokens[tokenCount++] = *token; 
        }
    }
    tokens = realloc(tokens, (tokenCount) * sizeof(Token));
    parser->tokenCount = tokenCount;
    return tokens;
}