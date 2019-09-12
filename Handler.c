//std lib
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//time
#include <time.h> // for srand

//written code files
#include "Queue.c"
#include "RandGenerator.c" // generate random num
#include "PriorityQueue.c"
// #include "ReadConfig.c"

//defined
#define IDLE 0
#define BUSY 1

#define QUEUESIZE 10
#define ARRIVE_MIN 2
#define ARRIVE_MAX 8

//define MIN MAX
#define CPU_MIN 50
#define CPU_MAX 70
#define DISK_MIN 80
#define DISK_MAX 160 // mb/s

void process_Arrival(Event* task, PQueue* eventQ);
void process_CPU(Event* task, PQueue* eventQ);
void process_DISK(Event* task, PQueue* eventQ);

// global vars
// idle 0
int cpu_status = 0;
int timeGlobal = 0;
int disk1_status = 0;
int disk2_status = 0;

// 3 queues for CPU DISK1 DISK 2
Node queue_CPU;
Node queue_DISK1;
Node queue_DISK2;

int main(){
    // Read data from file
    // readFile();
    // defineMacro();

    // Initial event qeueu and the event
    srand(time(NULL));
    Event* task;
    PQueue* EventQueue = initializePQ();

    // Initial queueFIFO
    // Queue size is 10 coded in queue.c func initializeQ()
    // CPU
    initializeQ(&queue_CPU);
    // DISK 1
    initializeQ(&queue_DISK1);
    // DISK 2
    initializeQ(&queue_DISK2);

    // initial file log.txt
    // FILE* fp = fopen("log.txt", "w");

    //create new to trigger
    Event job1;
    job1.jobSeq = 1;
    job1.type = 8; // Arrival
    // job1.time = randNum(CPU_MIN, CPU_MAX);
    job1.time = 0;
    // timeGlobal+= job1.time;
    push(EventQueue, &job1);

    while(isEmpty(EventQueue)&&(timeGlobal < FIN_TIME)){
        task = pop(EventQueue);
        usleep(500000);
        switch (task->type){
            case ARRIVAL: // cpu arrival
                timeGlobal = task->time;
                process_CPU(task, EventQueue);
                break;
            case CPU_BEGIN: // cpu start
                process_CPU(task, EventQueue);
                break;
            case CPU_FINISH: // cpu finish
                timeGlobal = task->time;
                process_CPU(task, EventQueue);
                break;
            case DISK_ARRIVAL: // disk arrival
                timeGlobal = task->time;
                process_DISK(task, EventQueue);
                break;
            case DISK1_BEGIN: // disk start
            case DISK2_BEGIN:
                process_DISK(task, EventQueue);
                break;
            // disk finish
            case DISK1_FINISH:
            case DISK2_FINISH: 
                timeGlobal = task->time;
                process_DISK(task, EventQueue);
                break;
            case 7: // end
                // FILE* fp = fopen("log.txt", "a");
                // fprintf(fp, "At time %-5d Job%d exits\n", timeGlobal, task->jobSeq);
                // fclose(fp);
                printf("At time %-5d Job%-3d exits\n", timeGlobal, task->jobSeq);
                break;
    } }
    // free memory
    destroy(EventQueue);
    return 1;
}
void process_Arrival(Event* task, PQueue* eventQ){
    printf("At time %-5d Job%-3d arrives\n", task->time, task->jobSeq);
    // enqueue to cpu queue
    enQueue(&queue_CPU, task->jobSeq);
    
    // 1. create new job
    Event next_job;
    next_job.jobSeq = task->jobSeq + 1;
    next_job.type = ARRIVAL; // arrival
    next_job.time = randNum(CPU_MIN, CPU_MAX) + timeGlobal;
    // next_job.time = task->time + 25; // test num
    
    // 2. enqueue new job to eventQueue
    push(eventQ, &next_job);
}

void process_CPU(Event* task, PQueue* eventQ){
    if(task->type == ARRIVAL){// arrival
        process_Arrival(task, eventQ);
    }

    if(task->type == CPU_BEGIN){ // begin
        // task finishes and exits
        Event task_fin;
        task_fin.jobSeq = task->jobSeq;
        task_fin.time = randNum(CPU_MIN, CPU_MAX) + timeGlobal;
        task_fin.type = CPU_FINISH;
        push(eventQ, &task_fin);
    }
    if(task->type == CPU_FINISH){ // finish
        // going to disk
        if(probabilityGet() == 0){
            // 1. create new event for going to disk
            Event disk_task;
            disk_task.jobSeq = task->jobSeq;
            disk_task.time = task->time;
            disk_task.type = DISK_ARRIVAL;

            // 2. insert the event
            push(eventQ, &disk_task);
        }else{
            Event all_fin;
            all_fin.jobSeq = task->jobSeq;
            all_fin.time = task->time;
            all_fin.type = END;
            push(eventQ, &all_fin);
        }
        cpu_status = IDLE;
        printf("At time %-5d job%-3d finishes at CPU\n",timeGlobal, task->jobSeq );
        
    }
    if( (queue_CPU.current>=1) && (cpu_status) == IDLE){
        // 1. pop out 1st in queue cpu
        int seq = deQueue(&queue_CPU);
        printf("At time %-5d job%-3d begins at CPU\n",task->time, seq );
        // 2. create cpu begin
        Event job_begin;
        job_begin.jobSeq = seq;
        job_begin.time = timeGlobal;
        job_begin.type = CPU_BEGIN; // begin cpu

        push(eventQ, &job_begin);

        cpu_status = BUSY;
    }
}
void process_Arrival_DISK(Event* task, PQueue* eventQ){
    printf("At time %-5d Job%-3d arrives at Disk\n", task->time, task->jobSeq);
    // enqueue to cpu queue
    int size1 = queue_DISK1.current;
    int size2 = queue_DISK2.current;
    if(disk1_status == IDLE && disk2_status == IDLE){
        if(size1 > size2){
            enQueue(&queue_DISK2, task->jobSeq);
        }else{
            enQueue(&queue_DISK1, task->jobSeq);
        }
    }else{
        if(disk1_status == IDLE && disk2_status == BUSY){
            enQueue(&queue_DISK1, task->jobSeq);
        }
        if(disk2_status == IDLE && disk1_status == BUSY){
            enQueue(&queue_DISK2, task->jobSeq);
        }
    }
    
}
void process_DISK(Event* task, PQueue* eventQ){
    // // CPU arrival
    switch (task->type)
    {
    case DISK_ARRIVAL:
        process_Arrival_DISK(task, eventQ);
        break;
    case DISK1_BEGIN :
    {
        Event task_fin1;
        task_fin1.jobSeq = task->jobSeq;
        task_fin1.time = randNum(DISK_MIN, DISK_MAX) + timeGlobal;
        task_fin1.type = DISK1_FINISH;
        push(eventQ, &task_fin1);

        break;
    }
    case DISK2_BEGIN :
    {
        Event task_fin2;
        task_fin2.jobSeq = task->jobSeq;
        task_fin2.time = randNum(DISK_MIN, DISK_MAX) + timeGlobal;
        task_fin2.type = DISK2_FINISH;
        push(eventQ, &task_fin2);

        break;
    }
    case DISK1_FINISH:
        enQueue(&queue_CPU, task->jobSeq);
        disk1_status = IDLE;
        break;

    case DISK2_FINISH:
        enQueue(&queue_CPU, task->jobSeq);
        disk2_status = IDLE;
        break;
    }
    if((queue_DISK1.current >= 1) && (disk1_status == IDLE)){
        // 1. pop out the 1st one
        int seq = deQueue(&queue_DISK1);
        printf("At time %-5d job%-3d begins at Disk1\n",task->time, seq );
        // 2. create disk begins
        Event job_begin;
        job_begin.jobSeq = seq;
        job_begin.time = timeGlobal;
        job_begin.type = DISK1_BEGIN;

        push(eventQ, &job_begin);
        disk1_status = BUSY;
    }
    if((queue_DISK2.current >= 1) && (disk2_status == IDLE)){
        // 1. pop out the 1st one
        int seq = deQueue(&queue_DISK2);
        printf("At time %-5d job%-3d begins at Disk2\n",task->time, seq );
        // 2. create disk begins
        Event job_begin;
        job_begin.jobSeq = seq;
        job_begin.time = timeGlobal;
        job_begin.type = DISK2_BEGIN;

        push(eventQ, &job_begin);
        disk2_status = BUSY;
    }
}
