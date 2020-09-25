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

long pTopSum(long randNums, int *numArray, int argc, char **argv, int np, int pid, MPI_Status status){

    int send, recv;
    int part = randNums/(np-1);
    int q = 0;
    int i;
    int val =1;
  
    if (pid == 0)
    {
        for (i = 1; i < np; i++)
        {
            send = part * (i-1);
            MPI_Send(&send, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

    }
    else
    {
        MPI_Recv(&recv, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        for (i = recv; i < recv + part; i++)
        {
            val = val+numArray[i];

        }
        MPI_Send(&val, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    if (pid == 0)
    {
        for(i = 1; i < np; i++){
            MPI_Recv(&val, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            q = q + val;
        }
    }
    if (pid == 0)
    {
        return q;
    }
 
    return 0;

}

long collectiveSum(long randNums, int *numArray, int argc, char **argv, int np, int pid){
    int load = randNums/(np-1);
    int tosum[load];
    int sums[np];
    int i;

    MPI_Scatter(numArray,load,MPI_INT, tosum, load, MPI_INT, 0, MPI_COMM_WORLD);

    sums[pid] = 0;
    for(i=0; i<np; i++){
        sums[pid] += tosum[i];
    }

    MPI_Gather(&sums[pid], 1, MPI_INT, sums, 1, MPI_INT, 0, MPI_COMM_WORLD);
  
    if(pid==0){
        for(i=1; i<np; i++){
            sums[0] += sums[i];
            return sums[0];
        }
     }
     return 0;

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

    int ptop = pTopSum(randNums, numArray, argc, argv, np, pid, status);
    printf("The ptop  sum is %d\n", ptop);
   
    clock_t end1 = clock();
    double time_spent1 = (double)(end1 - begin1);

    printf("Time spent in point to point: %f\n", time_spent1);

    // COLLECTIVE COMMUNICATION SUMMATION! 
    clock_t begin2 = clock();

    int collSum = collectiveSum(randNums, numArray, argc, argv, np, pid);
    printf("The collSum sum is %d\n", collSum);
   
    clock_t end2 = clock();
    double time_spent2 = (double)(end2 - begin2);

    printf("Time spent in collective communication: %f\n", time_spent2);

    MPI_Finalize();

    return 0;

}   
