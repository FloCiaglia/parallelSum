#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parallelSum.h"

#define MIN 0
#define MAX 5

int main(int argc, char **argv){

    long randNums;
    int *numArray;

    if(argc != 2){

        printf("Invalid number of argument\n");
        return 0;
    }

    randNums = atoi(argv[1]);
    randNums *= 100000;
    //printf("How many numbers?  %d\n", randNums);
    
    numArray = generateRands(MIN, MAX, randNums);
    /*int i;
    for(i=0; i < randNums; i++){
        printf("%d ", numArray[i]);
    }*/

    // START TIMING HERE!
    clock_t begin = clock();
    int sum = serialSum(randNums, numArray);
    printf("The sum is %d\n", sum);
   
    clock_t end = clock();
    double time_spent = (double)(end - begin);
    printf("Time spent: %f\n", time_spent);
    return 1;

}   
