//std lib
#include <stdio.h>
#include <stdlib.h>

//time
#include <time.h> // for srand

//written code files
#include "Queue.c"
#include "RandGenerator.c" // generate random num
#include "PriorityQueue.c"

//defined
#define QUEUESIZE 10
#define ARRIVE_MIN 2
#define ARRIVE_MAX 8
#define FIN_TIME 1000

// queue size
#define QUEUESIZE 10

//define MIN MAX
#define CPU_MIN 2
#define CPU_MAX 50
#define DISK_MIN 80
#define DISK_MAX 160 // mb/s

void process_CPU(Event* task, PQueue* eventQ);
void process_DISK(Event* task);

// idle 0
int cpu_status = 0;
int timeGlobal = 0;

// 3 queues for CPU DISK1 DISK 2

Node queue_CPU[QUEUESIZE];
Node queue_DISK1[QUEUESIZE];
Node queue_DISK2[QUEUESIZE];

int main(){
    srand(time(NULL));
    Event* task;
    int time;
    PQueue* EventQueue = initializePQ();

    //initial queueFIFO
    initializeQ(queue_CPU);
    initializeQ(queue_DISK1);
    initializeQ(queue_DISK2);

    //create new to trigger
    Event job1;
    job1.jobSeq = 1;
    job1.type = 2;
    job1.time = randNum(CPU_MIN, CPU_MAX);
    push(EventQueue, &job1);

    while(isEmpty(EventQueue)&&(time < FIN_TIME)){
        task = pop(EventQueue);
        time = task->time;
        switch (task->type){
            case 2: // cpu start
                process_CPU(task, EventQueue);
                break;
            case 3: // cpu finish
                process_CPU(task, EventQueue);
                break;
            case 4: // disk start
                process_DISK(task);
                break;
            case 5: // disk finish
                process_DISK(task);
                break;
            case 8: // end
                break;
    } }
    return 1;
}

void process_CPU(Event* task, PQueue* eventQ){
    if(task->type == 2){ // begin
        printf("job arrived");
        // 1. create new job
        Event next_job;
        next_job.jobSeq = task->jobSeq + 1;
        next_job.type = 2;
        next_job.time = randNum(CPU_MIN, CPU_MAX);
        
        // 2. enqueue new job to eventQueue
        push(eventQ, &next_job);

        // 3. send task to cpu
        enQueue(queue_CPU, task->jobSeq);
    }else{
        printf("job finished");
        // finished
        //idle=0
        cpu_status = 0; 
        if(!probablyGet(QUIT_PROB)){
            //going to disk
            printf("job goes to disk");

            // 1. create new event for going to disk
            Event disk_task;
            disk_task.jobSeq = task->jobSeq;
            disk_task.time = timeGlobal;
            disk_task.type = 4;

            // 2. insert the event
            push(eventQ, &disk_task);
        }
    }
    //check if cpu is idle
    if( (queue_CPU[0].current>=1) && (cpu_status) == 0){
        // 1. pop out 1st in queue cpu
        int seq = deQueue(queue_CPU);
        
        // 2. finish the event
        // time + processing time in cpu
        Event job_fin;
        job_fin.jobSeq = seq;
        job_fin.type = 3;
        job_fin.time = randNum(CPU_MIN, CPU_MAX);
        push(eventQ, &job_fin);
        cpu_status = 1;
    }
}

void process_DISK(Event* task){
    
}