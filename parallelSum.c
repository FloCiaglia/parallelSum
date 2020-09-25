#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
    int i;
    long sum = 0;

    for(i=0; i<randNums; i++){
        sum += numArray[i];
    }

    printf("The sum is %d\n", sum);
   
    clock_t end = clock();
    double time_spent = (double)(end - begin);
    printf("Time spent: %f\n", time_spent);
    return 1;

}   
