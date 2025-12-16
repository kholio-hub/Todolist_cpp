#include "queue.h"
#include <stdio.h>
#include <stdlib.h>


/*
 * Function: initQueue
 * -------------------------------------------------------
 * Description:
 *  - Initializes a circular queue.
 *  - Allocates dynamic memory for patient storage.
 *  - Resets queue control variables.
 *
 * Input:
 *  - q        : Pointer to Queue structure
 *  - capacity : Maximum number of patients the queue can hold
 *
 * Output:
 *  - None
 * -------------------------------------------------------
 */


void initQueue(Queue *q, int capacity) {

    /* Initialize queue indices to indicate empty state */
    q->front = -1;
    q->rear  = -1;

    /* Initialize current number of elements */
    q->size = 0;

    /* Set maximum queue capacity */
    q->capacity = capacity;

    /* Dynamically allocate memory for queue array */
    q->arr = (Patient*)malloc(capacity * sizeof(Patient));

    /* Check memory allocation success */
    if(q->arr == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);   // Critical failure → terminate program
    }
}

/*
 * Function: isEmpty
 * -------------------------------------------------------
 * Description:
 *  - Checks whether the queue is empty.
 *
 * Input:
 *  - q : Pointer to Queue structure
 *
 * Output:
 *  - Returns 1 if empty
 *  - Returns 0 otherwise
 * -------------------------------------------------------
 */

int isEmpty(Queue *q) {
    return (q->size == 0);
}

/*
 * Function: isFull
 * -------------------------------------------------------
 * Description:
 *  - Checks whether the queue is full.
 *
 * Input:
 *  - q : Pointer to Queue structure
 *
 * Output:
 *  - Returns 1 if full
 *  - Returns 0 otherwise
 * -------------------------------------------------------
 */

int isFull(Queue *q) {
    return (q->size == q->capacity);
}

/*
 * Function: enqueuePatient
 * -------------------------------------------------------
 * Description:
 *  - Inserts a patient into the circular queue.
 *  - Updates rear index using modulo arithmetic.
 *  - Increments queue size.
 *
 * Design Note:
 *  - This function does NOT print messages.
 *  - UI handling is done at the Application Layer.
 *
 * Input:
 *  - q : Pointer to Queue structure
 *  - p : Patient data to be inserted
 *
 * Output:
 *  - None
 * -------------------------------------------------------
 */


void enqueuePatient(Queue *q, Patient p) {

    /* Do nothing if queue is full */
    if(isFull(q)) {
        return;
    }

    /* If queue was empty, initialize front index */
    if(q->front == -1)
        q->front = 0;

    /* Move rear index circularly */
    q->rear = (q->rear + 1) % q->capacity;

    /* Insert patient at rear position */
    q->arr[q->rear] = p;

    /* Increment number of elements */
    q->size++;
}


/*
 * Function: dequeuePatient
 * -------------------------------------------------------
 * Description:
 *  - Removes and returns the front patient from the queue.
 *  - Updates front index using modulo arithmetic.
 *  - Resets queue if it becomes empty.
 *
 * Input:
 *  - q : Pointer to Queue structure
 *
 * Output:
 *  - Returns removed Patient
 *  - Returns invalid Patient (id = -1) if queue is empty
 * -------------------------------------------------------
 */


Patient dequeuePatient(Queue *q) {

    /* Default invalid patient */
    Patient temp = { -1, "", -1, 0 };

    /* Return invalid patient if queue is empty */
    if(isEmpty(q)) {
        return temp;
    }

    /* Get front patient */
    temp = q->arr[q->front];

    /* Move front index circularly */
    q->front = (q->front + 1) % q->capacity;

    /* Decrease size */
    q->size--;

    /* Reset queue indices if queue becomes empty */
    if(q->size == 0)
        q->front = q->rear = -1;

    return temp;
}


