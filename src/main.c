#include <string.h>
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
            printf("UNDEFINED\n");
            break;
    }
}

void decompile(Compiler *compiler) {
    for (int i = 0; i < compiler->operationCount; i++) {
        printf("%04lX\t", &compiler->operations[i] - compiler->operations);
        printOperation(compiler->operations[i]);
    }
}

void runProgram(const char* source, bool debug, Interpreter *interpreter) {
    size_t sourceLength = getSourceLength(source);
    // Create necessary objects.
    Parser parser;
    Compiler compiler;
    //Initialise stuff.
    initParser(&parser);
    initCompiler(&compiler);
    // Parse and Compile.
    Token* tokens = parse(&parser, source, sourceLength);
    compile(&compiler, tokens, parser.tokenCount);
    if (debug) {
        if (!isQueueEmpty(parser.input)) {
            printf("Input is: %s\n", parser.input->input);
        }
        decompile(&compiler);
    }
    // Interpret.
    interpret(interpreter, &compiler, &parser);
    // Free resources.
    freeCompiler(&compiler);
    freeParser(&parser);
}

void runFile(const char* fileName) {
    const char* source = getSource(fileName);
    Interpreter interpreter;
    initInterpreter(&interpreter);
    runProgram(source, false, &interpreter);
    freeInterpreter(&interpreter);
}

void repl() {
    printf("Brain-Fu Interpreter for Turing Machine\n");
    char *buffer = (char*) malloc(1024 * sizeof(char));
    bool debug = false;
    Interpreter interpreter;
    initInterpreter(&interpreter);
    do {
        printf("\nλ: ");
        scanf("%s", buffer);
        runProgram(buffer, debug, &interpreter);
        if (*buffer == 'd') {
            debug = !debug;
        }
    } while(*buffer != 'q');
    freeInterpreter(&interpreter);
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