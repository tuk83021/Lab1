#include <stdio.h>
#include <stdlib.h>

#define SIZE  11

int* temps;
unsigned int seed;
static char name[SIZE][15];

void readFile(){
    temps = malloc(SIZE * sizeof(int));
    char* filename = "config.txt";
    FILE* file = fopen(filename, "r");

    printf("Reading file %s...\n", filename);

    int count= 0;
    if (file != NULL) {
        if( fscanf(file, "%s %u\n", name[count], &seed) != EOF){
            printf("0. Read: %s %u\n",name[count], temps[count]);
        }
        while( fscanf(file, "%s %d\n", name[count], &temps[count]) != EOF){
            printf("%d. Read: %s %d\n", count+1,name[count], temps[count]);
            count+=1;
        }
    }else{
        perror("File failed to open");
        exit(1);
    }
    fclose(file);
    printf("File read complete.\n\n");
}

// Seed
unsigned int getSeed(){
    return seed;
}


// Init Time
int getIniTime(){
    return *(temps);
}

// Fin Time
int getFinTime(){
    return *(temps+1);
}

// Arrival min
int getArrivalMin(){
    return *(temps+2);
}

// Arrival max
int getArrivalMax(){
    return *(temps+3);
}

// Quit Prob
int getQuitProb(){
    return *(temps+4);
}

// CPU min
int getCPUMin(){
    return *(temps+5);
}

// CPU max
int getCPUMax(){
    return *(temps+6);
}

// Disk min
int getDisk1Min(){
    return *(temps+7);
}

// Disk max
int getDisk1Max(){
    return *(temps+8);
}

// Disk min
int getDisk2Min(){
    return *(temps+9);
}

// Disk max
int getDisk2Max(){
    return *(temps+10);
}

void destroyPtr(){
    free(temps);
}
