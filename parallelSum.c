#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>


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

long pToPSum(long randNums, int *numArray, int argc, char **argv, int np, int pid){

    


}

long collSum(long randNums, int *numArray, int argc, char **argv, int np, int pid){


}

int main(int argc, char **argv){
 
    int np, pid;
    
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

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

    if(pid==0){

        int serial = serialSum(randNums, numArray);
        printf("The serial sum is %d\n", serial);
    }
   
    clock_t end = clock();
    double time_spent = (double)(end - begin);

    printf("Time spent in serial: %f\n", time_spent);

    // POINT TO POINT SUMMATION!
    
    clock_t begin1 = clock();

    int ptop = pTopSum(randNums, numArray, argc, argv, np, pid);
    printf("The ptop  sum is %d\n", ptop);
   
    clock_t end1 = clock();
    double time_spent1 = (double)(end1 - begin1);

    printf("Time spent in point to point: %f\n", time_spent1);

    // COLLECTIVE COMMUNICATION SUMMATION! 
    clock_t begin2 = clock();

    int collSum = collSum(randNums, numArray, argc, argcv, np, pid);
    printf("The collSum sum is %d\n", collSum);
   
    clock_t end2 = clock();
    double time_spent2 = (double)(end2 - begin2);

    printf("Time spent in collective communication: %f\n", time_spent2);

    MPI_Finalize();

    return 0;

}   
