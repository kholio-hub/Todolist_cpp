#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_NAME 50

/**
 * Patient record used by the queues and higher layers.
 */
typedef struct {
    int id;
    char name[MAX_NAME];
    int age;
    int priority; /* 1 = Critical, 2 = Urgent, 3 = Normal */
} Patient;

typedef struct {
    int front;
    int rear;
    int size;
    int capacity;
    Patient *arr;
} Queue;

/* Queue API */
void initQueue(Queue *q, int capacity);
int isEmpty(Queue *q);
int isFull(Queue *q);
void enqueuePatient(Queue *q, Patient p);
Patient dequeuePatient(Queue *q);

#endif /* QUEUE_H */
