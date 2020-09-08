#ifndef BRAINFU_INPUT_H
#define BRAINFU_INPUT_H

#include "common.h"
#include <string.h>

typedef struct {
    char *input;
    char *current;
    int length;
} InputQueue;

/**
 * Dequeue a character from the input stack.
 *
 * @param queue Input queue to dequeue from.
 * @return the dequeued character.
 */
char dequeue(InputQueue *queue);
/**
 * Initialise an empty input queue.
 *
 * @param queue to initialise.
 */
void initInputQueue(InputQueue *queue);
/**
 * Concanate a string to the end of the string in queue.
 *
 * @param ququq eto concenate to.
 * @param input Input to add.
 * @param inputLength length of the input to add.
 */
void enqueueInput(InputQueue *queue, const char *input, int length);
/**
 * Check if the queue is empty.
 * @return true if the queue is empty.
 */
extern bool isQueueEmpty(InputQueue *queue);

#endif //BRAINFU_INPUT_H
