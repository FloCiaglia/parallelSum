//this is the header file

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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
