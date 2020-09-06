#ifndef brainfuc_interpreter_h
#define brainfuc_interpreter_h

#include "common.h"
#include "compiler.h"

typedef struct {
    int index;
    uint8_t value;
} Cell;

typedef struct {
    int capacity;
    int current;
    Cell *cells;
} Tape;

/**
 * Get the load factor of the tape,
 * load factor determines the chance of
 * collusions.
 * 
 * @param tape tape to get the load factor of.
 */
extern float getLoadFactor(Tape *tape);
/**
 * Initialise the tape.
 */
void initTape(Tape *tape);
/**
 * Get the hash index for a Cell
 * given the cells index.
 * 
 * @param tape tape to hash for.
 * @param index of the cell to get hash for.
 */
int getIndex(Tape *tape, int index);
/**
 * Get a cell pointer from the tape.
 * 
 * @param tape tape to get pointer from.
 * @param index index of the cell to get.
 * @return a pointer to the cell with the
 *  given index from the tape
 */
Cell *get(Tape *tape, int index);
uint8_t getValue(Tape *tape, int index);
bool isOnTape(Tape *tape, int index);
void *add(Tape *tape, int index, uint8_t value);


typedef struct {
    Tape *tape;
    bool isCached; // When this is true current cell is hold as a pointer.
    union { // If it is false,
        Cell *cachedCell;  // Current cell is instead stored as a
        int currentIndex;   // Integer value of its index.
    };
} Interpreter;

void initInterpreter(Interpreter *interpreter);
void freeInterpreter(Interpreter *Interpreter);

extern void cacheCell(Interpreter *interpreter);
extern uint8_t getCurrentValue(Interpreter *interpreter);
extern void modifyCurrentValue(Interpreter *interpreter, int modifier);
extern void moveOnTape(Interpreter *Interpreter, int movement);
extern Operation* jump(Interpreter *interpreter, Operation *localPointer, Operation *basePointer, int jumpTo);
void interpret(Interpreter *interpreter, Compiler *compiler);

#endif