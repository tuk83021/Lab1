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
#define FIN_TIME 5000

//define MIN MAX
#define CPU_MIN 50
#define CPU_MAX 100
#define DISK_MIN 80
#define DISK_MAX 160 // mb/s

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

// FILE *fp;
int main(){
    // fp = fopen("log.txt", "w+");
    srand(time(NULL));
    Event* task;
    PQueue* EventQueue = initializePQ();

    //initial queueFIFO
    // queue size is 10 coded in queue.c func initializeQ
    // cpu
    initializeQ(&queue_CPU);
    // disk 1
    initializeQ(&queue_DISK1);
    // disk 2
    initializeQ(&queue_DISK2);

    //create new to trigger
    Event job1;
    job1.jobSeq = 1;
    job1.type = 2;
    job1.time = randNum(CPU_MIN, CPU_MAX);
    timeGlobal+= job1.time;
    push(EventQueue, &job1);

    while(isEmpty(EventQueue)&&(timeGlobal < FIN_TIME)){
        // if(EventQueue->event->type >= 5){
        //     puts("-----event queue-----");
        //     printQ(EventQueue);
        //     puts("-----disk1 queue-----");
        //     toString(&queue_DISK1);
        //     puts("-----disk2 queue-----");
        //     toString(&queue_DISK2);
        //     break;
        // }
        task = pop(EventQueue);
        switch (task->type){
            case 2: // cpu start
                process_CPU(task, EventQueue);
                // printf("time: %d\n", timeGlobal);
                break;
            case 3: // cpu finish
                process_CPU(task, EventQueue);
                // printf("time: %d\n", timeGlobal);
                break;
            case 4: // disk start
                process_DISK(task, EventQueue);
                break;
            case 5:
            case 6: // disk finish
                process_DISK(task, EventQueue);
                break;
            case 7: // end
                // fprintf(fp, "At time %-4d Job%d exits\n", timeGlobal, task->jobSeq);
                printf("At time %-4d Job%-2d exits\n", timeGlobal, task->jobSeq);
                // fclose(fp);
                break;
    } }
    // fclose(fp);
    return 1;
}

void process_CPU(Event* task, PQueue* eventQ){
    if(task->type == 2){ // begin
        printf("At time %-4d Job%-2d arrives\n", timeGlobal, task->jobSeq);
        // 1. create new job
        Event next_job;
        next_job.jobSeq = task->jobSeq + 1;
        next_job.type = 2;
        next_job.time = randNum(CPU_MIN, CPU_MAX);
        timeGlobal += next_job.time;
        
        // 2. enqueue new job to eventQueue
        push(eventQ, &next_job);

        // 3. send task to cpu
        enQueue(&queue_CPU, task->jobSeq);
    }else{ // != 2
        printf("At time %-4d Job%-2d finishes at CPU\n", timeGlobal, task->jobSeq);
        // finished
        //idle=0
        cpu_status = 0; 
        if(probablyGet(QUIT_PROB) == 0){
            //going to disk
            // 1. create new event for going to disk
            Event disk_task;
            disk_task.jobSeq = task->jobSeq;
            disk_task.time = randNum(DISK_MIN, DISK_MAX);
            timeGlobal+= disk_task.time;
            disk_task.type = 4;

            // 2. insert the event
            push(eventQ, &disk_task);
        }else{
            printf("At time %-4d Job%-2d exits\n", timeGlobal, task->jobSeq);
        }
    }
    //check if cpu is idle
    if( (queue_CPU.current>=1) && (cpu_status) == 0){
        // 1. pop out 1st in queue cpu
        int seq = deQueue(&queue_CPU);
        
        // 2. finish the event
        // time + processing time in cpu
        Event job_fin;
        job_fin.jobSeq = seq;
        job_fin.type = 3;
        job_fin.time = randNum(CPU_MIN, CPU_MAX);
        timeGlobal += job_fin.time;
        push(eventQ, &job_fin);
        cpu_status = 1;
    }
}

void process_DISK(Event* task, PQueue* eventQ){

    // arrive at disk section
    if(task->type == 4){
        // fprintf(fp, "At time %-4d Job%d arrives at Disk\n", timeGlobal, task->jobSeq);
        printf("At time %-4d Job%-2d arrives at Disk\n", timeGlobal, task->jobSeq);
        // 1. select disk
        int size1 = queue_DISK1.current;
        int size2 = queue_DISK2.current;
        if(size1 < size2){
            // 2.1 send job to disk1
            enQueue(&queue_DISK1, task->jobSeq);
        }else{
            // 2.2 send job to disk2
            enQueue(&queue_DISK2, task->jobSeq);
        }
    }else{
        // finish at disk

        // finish at disk 1
        if(task->type == 5){
            // fprintf(fp, "At time %-4d Job%d finishes IO at Disk1\n", timeGlobal, task->jobSeq);
            printf("At time %-4d Job%-2d finishes IO at Disk1\n", timeGlobal, task->jobSeq);
            // 1. create new event for going to disk
            Event task1;
            task1.jobSeq = task->jobSeq + 1;
            task1.time = randNum(DISK_MIN, DISK_MAX);
            timeGlobal+= task1.time;
            task1.type = 2;

            // 2. insert the event
            push(eventQ, &task1);

            // 3. set disk1 to idle
            disk1_status = 0;
        }
        // finish at disk 2
        if(task->type == 6){
            // fprintf(fp, "At time %-4d Job%d finishes IO at Disk2\n", timeGlobal, task->jobSeq);
            printf("At time %-4d Job%-2d finishes IO at Disk2\n", timeGlobal, task->jobSeq);
            // 1. create new event for going to disk
            Event task2;
            task2.jobSeq = task->jobSeq + 1;
            task2.time = randNum(DISK_MIN, DISK_MAX);
            timeGlobal+= task2.time;
            task2.type = 2;

            // 2. insert the event
            push(eventQ, &task2);

            // 3. set disk1 to idle
            disk2_status = 0;
        }
    }


    if((queue_DISK1.current>=1) && (disk1_status) == 0){
        // 1. pop out 1st in queue disk1
        int seq = deQueue(&queue_DISK1);

        // 2. finish the event
        Event job_fin;
        job_fin.jobSeq = seq;
        job_fin.type = 7;
        job_fin.time = randNum(DISK_MIN, DISK_MIN);
        timeGlobal += job_fin.time;
        push(eventQ, &job_fin);
        
        // 3. set to busy
        disk1_status = 1;

    }
    if((queue_DISK2.current>=1) && (disk2_status) == 0){
        // 1. pop out 1st in queue disk2
        int seq = deQueue(&queue_DISK2);

        // 2. finish the event
        Event job_fin;
        job_fin.jobSeq = seq;
        job_fin.type = 7;
        job_fin.time = randNum(DISK_MIN, DISK_MIN);
        timeGlobal += job_fin.time;
        push(eventQ, &job_fin);
        
        // 3. set to busy
        disk2_status = 1;
    }
}