#include "interpreter.h"

int getIndex(Tape *tape, int index) {
    return index % tape->capacity;
}


/**
 * Fill a tape with
 * "empty" Cells, an empty cell
 * is represented as a cell whose index
 * is -1.
 */
void fillTape(Tape *tape) {
    for (Cell *cell = tape->cells; cell < tape->cells + tape->capacity; cell++) {
        Cell *newCell = malloc(sizeof(Cell));
        newCell->index = -1;
        newCell->value = 0;
        *cell = *newCell;
    }
}

void initTape(Tape *tape) {
    tape->capacity = 8;
    tape->current = 0;
    tape->cells = (Cell*) malloc(tape->capacity * sizeof(Cell));
    fillTape(tape);
}

void freeTape(Tape *tape) {
    free(tape->cells);
}

Cell *get(Tape *tape, int index) {
    Cell* cell = tape->cells + getIndex(tape, index);
    while (cell < tape->cells + tape->capacity && cell->index != -1) {
        if (cell->index == index) {
            return cell;
        }
        cell++;
    }
    return NULL;
}

bool isOnTape(Tape *tape, int index) {
    return get(tape, index) != NULL;
}

uint8_t getValue(Tape *tape, int index) {
    if (!isOnTape(tape, index)) {
        return 0;
    } else {
        return get(tape, index)->value;
    }
}


inline float getLoadFactor(Tape *tape) {
    return (float) tape->current / (float) tape->capacity;
}

void copyTape(Tape *origin, Tape *destination) {
    for (Cell *cell = origin->cells; cell < origin->cells + origin->capacity; cell++) {
        if (cell->index != -1 && cell->value != 0) {
            add(destination, cell->index, cell->value);
        }
    } 
}

void increaseTape(Tape *tape) {
    Cell *newCells = (Cell *) malloc(tape->capacity * 2 * sizeof(Cell *));
    Tape *newTape = (Tape *) malloc(sizeof(Tape));
    newTape->capacity = tape->capacity * 2;
    newTape->current =  0;
    newTape->cells = newCells;
    fillTape(newTape);
    copyTape(tape, newTape);
    freeTape(tape);
    *tape = *newTape;
}

bool add(Tape *tape, int index, uint8_t value) {
    bool increased = false;
    if (getLoadFactor(tape) > 0.75) {
        increaseTape(tape);
        increased = true;
    }
    int lookAt = getIndex(tape, index); // Where to start looking at.
    Cell* cell = tape->cells + lookAt; // Traversing cell pointer.
    Cell* limit = tape->cells + tape->capacity; // Limit to go for.
    while (cell < limit && cell->index != -1) {
        cell++;
    }
    if (cell == limit) { // If we couldn't find a slot.
        increaseTape(tape); // Increase the tape.
        add(tape, index, value); // Try adding it again.
        increased = true;
    } else { // Otherwise, we must have found an empty spot!
        cell->index = index;
        cell->value = value; // Add the new cell.
        tape->current++;
    }
    return increased;
}

inline void cacheCell(Interpreter *interpreter) {
    interpreter->cachedCell = get(interpreter->tape, interpreter->currentIndex);
    interpreter->isCached = true;
}

inline uint8_t getCurrentValue(Interpreter *interpreter) {
    if (interpreter->isCached) {
        return interpreter->cachedCell->value;
    } else {
        return getValue(interpreter->tape, interpreter->currentIndex);
    }
}

inline void modifyCurrentValue(Interpreter *interpreter, int modifier) {
    if (!interpreter->isCached) { // If the object isn't cached
        if (!isOnTape(interpreter->tape, interpreter->currentIndex)) { // but first If the cell isn't created yet.
            add(interpreter->tape, interpreter->currentIndex, 0); // Create it.
        }
        cacheCell(interpreter); // [then] Cache it
    }
    interpreter->cachedCell->value += modifier; //  Modify its value.
}

inline void moveOnTape(Interpreter *interpreter, int movement) {
    int newIndex;
    if (interpreter->isCached) {
        newIndex = interpreter->cachedCell->index + movement;
        interpreter->isCached = false;
    } else {
        newIndex = interpreter->currentIndex + movement;
    }
    if (newIndex <= 0) {
        newIndex = 0;
    }
    interpreter->currentIndex = newIndex;  // Get to the new index.
}

inline Operation **jump(Interpreter *interpreter, Operation **localPointer, Operation **basePointer, int address) {
    unsigned long offset = (localPointer - basePointer);
    if ((address < offset && getCurrentValue(interpreter) != 0)
        || (address > offset && getCurrentValue(interpreter) == 0)) { // Means a backward jump ].
        return basePointer + address;
    } else {
        return localPointer;
    }
}

void initInterpreter(Interpreter *interpreter) {
    interpreter->isCached = false;
    interpreter->currentIndex = 0;
    Tape *tape = (Tape*) malloc(sizeof(Tape));
    initTape(tape);
    interpreter->tape = tape;
}

void freeInterpreter(Interpreter *interpreter) {
    freeTape(interpreter->tape);
}

void interpret(Interpreter *interpreter, Compiler *compiler) {
    Operation** opPointer = compiler->operations;
    Operation** limit = opPointer + compiler->operationCount;
    Operation *currentOperation;
    while (opPointer < limit) {
        currentOperation = *opPointer;
        switch (currentOperation->op_code)
        {
        case OP_OUTPUT:
            printf("%c", getCurrentValue(interpreter));
            break;
        case OP_MOVE:
            moveOnTape(interpreter, currentOperation->operand);
            break;
        case OP_JUMP:
            opPointer = jump(interpreter, opPointer, compiler->operations, currentOperation->operand);
            break;
        case OP_CHANGE:
            modifyCurrentValue(interpreter, currentOperation->operand);
            break;
        case OP_INPUT:
            break;
        default:
            reportError("Unexpected operation bytecode.");
            break;
        }
        opPointer++;
    }
}
