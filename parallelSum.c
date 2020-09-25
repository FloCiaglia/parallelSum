#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parallelSum.h"

#define MIN 0
#define MAX 5

int* generateRands(int lower, int upper, int count){
    int i;
    int *num_array = (int *)malloc(sizeof(int)*count);

    if(num_array == NULL){
        printf("malloc failed");
        exit(1);
    }


    for(i = 0; i < count; i++){
       num_array[i] = (rand() % (upper-lower+1))+lower;

    }

    return num_array;
}


long serialSum(long randNums, int *numArray){

    int i;
    long sum = 0;

    for(i=0; i<randNums; i++){
        sum += numArray[i];
    }

    return sum;
}

long pToPSum(long randNums, int *numArray){

    


}


int main(int argc, char **argv){

    long randNums;
    int *numArray;

    if(argc != 2){

        printf("Invalid number of argument\n");
        return 1;
    }

    randNums = atoi(argv[1]);
    randNums *= 100000;
    
    numArray = generateRands(MIN, MAX, randNums);
    /*int i;
    for(i=0; i < randNums; i++){
        printf("%d ", numArray[i]);
    }*/

    // SERIAL SUMMATION!
    clock_t begin = clock();

    int serial = serialSum(randNums, numArray);

    printf("The sum is %d\n", serial);
   
    clock_t end = clock();
    double time_spent = (double)(end - begin);

    printf("Time spent in serial: %f\n", time_spent);

    // POINT TO POINT SUMMATION!
    


    return 0;

}   
