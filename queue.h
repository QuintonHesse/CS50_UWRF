/* We use ifndef, define, and endif preprocessor directives here.
 * Why? See: https://stackoverflow.com/questions/1653958/why-are-ifndef-and-define-used-in-c-header-files
 */

#ifndef QUEUE_H
#define QUEUE_H

typedef struct Node {
   char *data;
   struct Node *next;
} Node;


void printQueue();

// enqueue and dequeue should add and remove strings from the queue using FIFO.

// enqueue adds a string the queue
void enqueue(char *str);

// enqueue removes and returns a string from the queue
char* dequeue();

#endif
