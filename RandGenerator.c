#include <stdio.h>
#define QUIT_PROB 20

int randNum(int low, int high){
    if((high-low) < 0 || high < 0 || low < 0){
        perror("Invalid input");
    }
    return rand() % (high - low + 1) + low;
}

// chance to get into disk
// 1 for true;
// 0 for false
int probabilityGet(){
    if(QUIT_PROB < 0 || QUIT_PROB > 100){
        perror("Invalid");
        return -1;
    }
    int prob = randNum(1, 100);
    if(prob <= QUIT_PROB){
        return 0;//goto disk
    }
    return 1;//exit
    
}

// int main(){
//     srand(time(NULL));
//     for(int i = 0; i < 10; i++){
//         printf("Rand %d -> %d\n", i, randNum(2,8));
//     }
//     return 1;
// }