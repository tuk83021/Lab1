#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int current;
    int sequence[10];
    int size;
} Node;

void initializeQ(Node *queue)
{
    if(queue == NULL){
        return;
    }
    queue->current = 0;
    queue->size = 10;
    for (int i = 0; i < queue->size; i++)
    {
        (queue->sequence)[i] = 0;
    }
}

int enQueue(Node *queue, int jobSeq)
{
    int size = queue->size;
    int *seq = queue->sequence;
    if (queue->current == size)
    {
        return 0;
    }
    seq[queue->current] = jobSeq;
    queue->current += 1;
    return 1;
}

int queueAdjust(int *arr, int *size)
{
    for (int i = 0; i < *size - 1; i++)
    {
        arr[i] = arr[i + 1];
    }
    arr[*size - 1] = 0;
    *size -= 1;
    return 1;
}

int deQueue(Node *queue)
{
    if (queue == NULL)
    {
        return 0;
    }
    int del = queue->sequence[0];
    queueAdjust(queue->sequence, &queue->current);
    return del;
}



void toString(Node *queue)
{

    for (int i = 0; i < queue->size; i++)
    {
        printf("Job:%d\n", queue->sequence[i]);
    }
}

// int main(void){
//     Node queue[10];
//     initializeQ(queue);
//     enQueue(queue, 1);
//     enQueue(queue, 2);
//     enQueue(queue, 3);
//     enQueue(queue, 4);
//     enQueue(queue, 5);
//     enQueue(queue, 6);
//     enQueue(queue, 7);
//     enQueue(queue, 8);
//     enQueue(queue, 9);
//     // enQueue(queue, 10);
//     enQueue(queue, 11);
//     enQueue(queue, 12);
//     enQueue(queue, 13);

//     deQueue(queue);
//     deQueue(queue);
//     toString(queue);
//     return 1;
// }