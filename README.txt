CIS 3207 Project 1 Giorgio's Discrete Event Simulator


File Handler.c


--Func void process_Arrival(Event* task, PQueue* eventQ);

Event* task: the event that is taken out of event queue
PQueue* eventQ: the event queue which contains all event that is needed to be poped out

Function processes the event that is poped out from event queue and take it into cpu queue 
as well as creating a new event

--Func void process_CPU(Event* task, PQueue* eventQ);

Event* task: the event that is taken out of event queue
PQueue* eventQ: the event queue which contains all event that is needed to be poped out

Function deals with the current task that is related to CPU part. 
1. turn into cpu arrival into cpu finish, and calculate the time spent in
2. check if the finished cpu task goes into disk based on the quit_prob

--Func void process_DISK(Event* task, PQueue* eventQ);

Event* task: the event that is taken out of event queue
PQueue* eventQ: the event queue which contains all event that is needed to be poped out

Function checks which disk the current task goes to,
        the first one in queue goes processing, 
        and push finished task into cpu queue
    
--Func void fileRead();

Function reads data from File, config.txt, makes correlated var linked to the data.


File PriorityQueue.c


Functions: 

--PQueue* initializePQ();

Function initialize event queue which is the priority queue. It pushes the init_time 
    and fin_time into the queue. It returns the location of the event queue.

--int isFull(PQueue* heap);

Function checks if the event queue is full or not. It returns the current size of the queue.

return 1 if true, 0 for false.

--void push(PQueue* heap, Event* one);

PQueue* heap: event queue
Event* one: one specific event

Function pushes the new event which is one into the event queue, it will sort the queue based on the time priority.

--Event* pop(PQueue* heap);

PQueue* heap: event queue.

Function popes out the first event.

return the poped out event

--void destroy(PQueue* heap);

PQueue* heap: event queue.

Function frees out the malloced ptrs

--int isEmpty(PQueue* heap);

PQueue* heap: event queue.

Function checks if the event queue is empty.

return the current size of event queue

--void printQ(PQueue* heap);

PQueue* heap: event queue.

Function prints out each event.


File RandFenerator.c


Functions:

--int randNum(int low, int high);

int low: low bound
int high: high bound

Function generates random number with range from low to high

--int probabilityGet(int QUIT_PROB);

int QUIT_PROB: the percent out of 100 goes to disk

Function generates a random number uesed previous func. If it is less than QUIT_PROB,
    it means the event poped out goes to disk.

return int


File ReadConfig.c


Functions:

--void readFile();

Function reads data from "config.txt", assigns each data to two arrays.