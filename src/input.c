
#include "input.h"

void initInputQueue(InputQueue *queue) {
    queue->input = (char*) malloc(sizeof(char)); // \0.
    *queue->input = '\0';
    queue->current = queue->input;
    queue->length = 0;
}

inline bool isQueueEmpty(InputQueue *queue) {
    return queue->length == 0;
}


void enqueueInput(InputQueue *queue, const char *input, int length) {
    if (isQueueEmpty(queue)) {
        queue->input = (char *) realloc(queue->input, length * sizeof(char));
        memcpy(queue->input, input, length);
        queue->current =  queue->input;
        queue->length = length;
    } else {
        queue->input = (char *) realloc(queue->input, (length + queue->length) * sizeof(char));
        queue->input[queue->length - 1] = '!'; // Replace previous '\0' with '!' since it was discarded by parser.
        memcpy(&queue->input[queue->length], input, length); // Add the reminder part to the end.
        queue->length += length;
    }
}

char dequeue(InputQueue *queue) {
    if (isQueueEmpty(queue)) {
        return '\0';
    }
    char returnValue = *queue->current;
    queue->current++;
    return returnValue;
}
