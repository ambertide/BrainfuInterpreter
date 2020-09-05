#include "common.h"
#include "parser.h"
#include "compiler.h"

void printOperation(Operation *operation) {
    printf("%h\t\t%l\n", operation->op_code, operation->operand);
}

void decompile(Compiler *compiler) {
    for (int i = 0; i < compiler->operationCount; i++) {
        printf("%h \t", &compiler->operations[i] - compiler->operations);
        printOperation(&compiler->operations[i]);
    }
}

void runProgram(const char* source, bool debug) {
    size_t sourceLength = getSourceLength(source);
    Parser *parser;
    Compiler *compiler;
    initParser(parser);
    initCompiler(compiler);
    Token* tokens = parse(parser, source, sourceLength);
    compile(compiler, tokens, parser->tokenCount);
    if (debug) {
        decompile(compiler);
    }
    freeCompiler(compiler);
    freeParser(parser);
}

void runFile(const char* fileName) {
    const char* source = getSource(fileName);
    runProgram(source, false);
}

void repl() {
    char *buffer = (char*) malloc(1024 * sizeof(char));
    do {
        scanf("%s", buffer);
        printf("λ: ");
        runProgram(buffer, true);
    } while(buffer[0] != 'q');
}

int main(int argc, char* argv[]) {
    printf("AAA");
    if (argc == 1) {
        runFile(argv[0]);
    } else if (argc == 0) {
        repl();
    } else {
        reportError("Wrong format.");
    }
    return 0;
}