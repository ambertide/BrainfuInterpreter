#include "compiler.h"


Operation *emitByteCode(Compiler *compiler, ByteCode byteCode, int operand) {
    if (compiler->operationCount == compiler->operationCapacity) {
        compiler->operationCapacity *= 2; // Resize the dynamic array.
        compiler->operations = (Operation**) realloc(compiler->operations, (compiler->operationCapacity * sizeof(Operation*)));
    }
    Operation *newOperation = malloc(sizeof(Operation));
    newOperation->op_code = byteCode;
    newOperation->operand = operand;
    *(compiler->operations + compiler->operationCount) = newOperation;
    compiler->operationCount++;
    return newOperation;
}


Token* compileChange(Compiler *compiler, Token *token, Token *limit) {
    int totalChange = 0;
    while (token < limit) {
        switch (*token)
        {
        case TOKEN_INCREMENT:
            totalChange++;\
            token++;
            continue;
        case TOKEN_DECREMENT:
            totalChange--;
            token++;
            continue;
        default:
            break;
        }
        break;
    }

    if (totalChange != 0) {
        emitByteCode(compiler, OP_CHANGE, totalChange);
    }
    return token;
}


Token* compileMove(Compiler *compiler, Token *token, Token *limit) {
    int totalChange = 0;
    while (token < limit) {
        switch (*token)
        {
        case TOKEN_RIGHT:
            totalChange++;\
            token++;
            continue;
        case TOKEN_LEFT:
            totalChange--;
            token++;
            continue;
        default:
            break;
        }
        break;
    }
    if (totalChange != 0) {
        emitByteCode(compiler, OP_MOVE, totalChange);
    }
    return token;
}

void initStack(JumpStack *stack) {
    stack->capacity = 8;
    stack->current = 0;
    stack->operationPointers = (Operation**) malloc(stack->capacity * sizeof(Operation*));
}

void checkStackSize(JumpStack *stack) {
    if (stack->current == stack->capacity) {
        stack->capacity *= 2;
        stack->operationPointers = (Operation**) realloc(stack->operationPointers, stack->capacity * sizeof(Operation*));
    }
}

bool isEmpty(JumpStack *stack) {
    return stack->current == 0;
}

void push(JumpStack *stack, Operation *ptr) {
    checkStackSize(stack);
    stack->operationPointers[stack->current++] = ptr;
}

Operation* pop(JumpStack *stack) {
    if (stack->current == 0) {
        return NULL;
    }
    stack->current--;
    return stack->operationPointers[stack->current];
}

void freeStack(JumpStack *stack) {
    free(stack->operationPointers);
}

void initCompiler(Compiler *compiler) {
    compiler->operationCapacity = 8;
    compiler->operationCount = 0;
    compiler->operations = (Operation**) malloc(compiler->operationCapacity * sizeof(Operation*));
}

void freeCompiler(Compiler *compiler) {
    free(compiler->operations);
}

void compile(Compiler *compiler, Token *tokens, size_t tokenCount) {
    JumpStack forwardsStack;
    initStack(&forwardsStack);
    Token *token = tokens;
    Token* limit = tokens + tokenCount;
    while (token < tokens + tokenCount) {
        switch (*token)
        {
        case TOKEN_INCREMENT:
        case TOKEN_DECREMENT:
            token = compileChange(compiler, token, limit);
            break;
        case TOKEN_RIGHT:
        case TOKEN_LEFT:
            token = compileMove(compiler, token, limit);
            break;
        case TOKEN_INPUT:
            emitByteCode(compiler, OP_INPUT, 0);
            token++;
            break;
        case TOKEN_OUTPUT:
            emitByteCode(compiler, OP_OUTPUT, 0);
            token++;
            break;
        case TOKEN_FORWARD:
            ;
            Operation *operation = emitByteCode(compiler, OP_JUMP, compiler->operationCount); // We put the current address as placeholder.
            push(&forwardsStack, operation);
            token++;
            break;
        case TOKEN_BACKWARD:
            ;
            Operation *corresponding = pop(&forwardsStack); // We pop the matching [ from the stack.
            if (corresponding == NULL) {
                reportError("There is a ] without a matching [.");
            } else {
                emitByteCode(compiler, OP_JUMP, corresponding->operand);
                corresponding->operand = compiler->operationCount - 1; // We now found the corresponding ]'s location.
            }
            token++;
            break;
        default:
            break;
        }
    }
    if (!isEmpty(&forwardsStack)) {
        reportError("There is a [ without a matching ].");
    }
}

#undef CALCULATE_DELTA