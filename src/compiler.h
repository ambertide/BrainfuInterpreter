#ifndef brainfuc_compiler_h
#define brainfuc_compiler_h

#include "common.h"
#include "parser.h"




typedef enum {
    OP_CHANGE, // For the current tape cell's value.
    OP_INPUT, // For inputting.
    OP_OUTPUT, // For outputing.
    OP_JUMP, // For moving on the symbols.
    OP_MOVE // For moving on the tape.
} ByteCode;

typedef struct {
    ByteCode op_code;
    int operand;
} Operation;

typedef struct
{
    int capacity;
    int current;
    Operation **operation_ptrs;
} JumpStack;

void initStack(JumpStack *stack);
void push(JumpStack *stack, Operation *ptr);
Operation* pop(JumpStack *stack);
/**
 * Free the stack.
 */
void freeStack(JumpStack *stack);
/**
 * Check and if neccesary increase the
 * stack size.
 */
void checkStackSize(JumpStack *stack);
/**
 * @return true if the stack is empty.
 */
bool isEmpty(JumpStack *stack);

typedef struct {
    int operationCount;
    int operationCapacity;
    Operation *operations;
} Compiler;

/**
 * Emit a byte code by adding the bytecode
 * with necessary operands to the operations
 * array, resize it if necessary.
 * 
 * @param compiler Compiler instance to add the bytecode.
 * @param byteCode Op code of the operation.
 * @param operand operand of the operation.
 * @return a pointer to the newly created operation object.
 */
Operation* emitByteCode(Compiler *compiler, ByteCode byteCode, int operand);

void initCompiler(Compiler *compiler);
void freeCompiler(Compiler *complier);
void compile(Compiler *compiler, Token *tokens, size_t tokenCount);

#endif