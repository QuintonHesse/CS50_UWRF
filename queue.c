#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./queue.h"

// TODO 1: Create Node pointers to the first and last node in the queue
//         Make sure to initialize the pointers to NULL.
//         After this step, you should be able to compile and run the code.
Node* first = NULL;
Node* last = NULL;

// print queue from first in line to last in line
void printQueue() {
    Node* currentNode = first;

    while( currentNode != NULL) {
        printf("%s\n", currentNode->data );
        currentNode = currentNode->next;
    }
}

void enqueue(char *str) {
    // TODO 2: Create a new Node using malloc.
    //         Initialize the Node's next pointer to NULL.
    //         Initialize the Node's char pointer (data) to str.
    Node* enqNode = (Node *) malloc(sizeof(Node));
    enqNode->next = NULL;
    enqNode->data = str;

    if (first == NULL)
    {
        first = enqNode;

        last = enqNode;
        // TODO 3: if first is NULL, the queue is empty
        //         assign first and last to the new Node.
        return;
    }
    else{
        last->next = enqNode;
        last = enqNode;

    }



    // TODO 4: If we make it this far, the queue is non-empty.
    //         Make the new Node the last node in the queue.
}

char* dequeue(){
    if (first == NULL)
    {
        return NULL;
        // TODO 5: The queue is empty, so return NULL.
    }

    if (first == last)
    {
        Node *tmp = last;
        char *ret = last->data;
        first = NULL;
        last = NULL;
        free(tmp);
        return ret;
        // TODO 6: The queue contains 1 Node.
        //         Return the data of the Node and free the Node. This takes a bit of logic.
        //         Be sure to set first and last pointers to NULL.
    }
    else {
        Node *tmp = first;
        char *ret = first->data;
        first = first->next;
        free(tmp);
        return ret;
    }

    // TODO 7: If we make it this far, the queue contains 2 or more elements.
    //         Return the data of the first, free the first Node, and update the
    //         first Node to reflect that the old first Node was removed.
    //         This takes a bit of logic to get right.
