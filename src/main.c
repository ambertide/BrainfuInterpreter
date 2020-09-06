#include "common.h"
#include "parser.h"
#include "compiler.h"
#include "interpreter.h"


void printOperation(Operation *operation) {
    switch(operation->op_code) {
        case 0:
            printf("OP_CHANGE\t\t%i\n", operation->operand);
            break;
        case 1:
            printf("OP_INPUT \t\t%i\n", operation->operand);
            break;
        case 2:
            printf("OP_OUTPUT\t\t%i\n", operation->operand);
            break;
        case 3:
            printf("OP_JUMP  \t\t%04X\n", operation->operand);
            break;
        case 4:
            printf("OP_MOVE  \t\t%i\n", operation->operand);
            break;
        default:
            printf("UNDEFINED");
            break;
    }
}

void decompile(Compiler *compiler) {
    for (int i = 0; i < compiler->operationCount; i++) {
        printf("%04lX\t", &compiler->operations[i] - compiler->operations);
        printOperation(&compiler->operations[i]);
    }
}

void runProgram(const char* source, bool debug) {
    size_t sourceLength = getSourceLength(source);
    Parser parser;
    Compiler compiler;
    initParser(&parser);
    initCompiler(&compiler);
    Token* tokens = parse(&parser, source, sourceLength);
    compile(&compiler, tokens, parser.tokenCount);
    if (debug) {
        decompile(&compiler);
    }
    
    freeCompiler(&compiler);
    freeParser(&parser);
}

void runFile(const char* fileName) {
    const char* source = getSource(fileName);
    runProgram(source, true);
}

void repl() {
    printf("Brain-Fu Interpreter for Turing Machine\n\n");
    char *buffer = (char*) malloc(1024 * sizeof(char));
    do {
        printf("λ: ");
        scanf("%s", buffer);
        runProgram(buffer, true);
    } while(buffer[0] != 'q');
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        printf("Running file %s\n", argv[1]);
        runFile(argv[1]);
    } else if (argc == 1) {
        repl();
    } else {
        reportError("Wrong format.");
    }
    return 0;
}