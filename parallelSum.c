#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/time.h>
#include <time.h>

#define MIN 0
#define MAX 5

//Citing Sources: https://www.geeksforgeeks.org/generating-random-number-range-c/
//                http://homepages.math.uic.edu/~jan/mcs572/using_mpi.pdf
//                http://homepages.math.uic.edu/~jan/mcs572/mcs572notes/lec05.html


int* generateRands(int lower, int upper, int count){
    int i;
    int *num_array = (int *)malloc(sizeof(int)*count);

    if(num_array == NULL){
        printf("malloc failed");
        exit(1);
    }

    srand(53);
    for(i = 0; i < count; i++){
       num_array[i] = (rand() % (upper-lower+1))+lower;

    }

    return num_array;
}


long long serialSum(long randNums, int *numArray){

    int i;
    long sum = 0;

    for(i=0; i<randNums; i++){
        sum += numArray[i];
    }

    return sum;
}

long long pTopSum(long randNums, int *numArray, int np, int pid, MPI_Status status){

    int load = randNums/np;
    int excess = randNums % np;
    int sum = 0;
    int i;

    int ptp_sum = 0;

    if (pid == 0)
    {
        for (i = 0; i < load; i++)
        {
            ptp_sum += numArray[pid*load+i];
        }
        for(i = 1; i < np; i++){
            MPI_Recv(&sum, 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
            ptp_sum += sum;
        }
    }else{
        if(pid == (np-1)){
            for(i = 0; i < (load + excess); i++){
                sum += numArray[pid*load+i];
            }
        }else{
            for(i = 0; i < load; i++){
                sum += numArray[pid*load+i];
            }

        }

        MPI_Send(&sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    if(pid == 0){
        return ptp_sum;
    }
    return 0;
}

long long collectiveSum(long randNums, int *numArray, int np, int pid){

    int load = randNums/np;
    long long coll_sum = 0;
    int i;
    int *output = (int*) malloc(sizeof(int)* randNums);
    int excess = randNums % np;


    if(pid == (np-1)){
        for(i = 0; i < (load+excess); i++){
            coll_sum += numArray[pid*load+i];
        }

    }else{
        for(i =0; i < load; i++){
            coll_sum += numArray[pid*load+i];
        }
    }

    MPI_Reduce(&coll_sum, output, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if(pid == 0){
        return output[0];
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

    // SERIAL SUMMATION!
    clock_t begin = clock();

    if(pid==0){

        long long serial = serialSum(randNums, numArray);
        printf("The serial sum is %d\n", serial);

        clock_t end = clock();
        double time_spent = (double)(end - begin);
        

        printf("Time spent in serial: %f\n", (float)time_spent/CLOCKS_PER_SEC);
        printf("\n");
    }
   
    MPI_Barrier(MPI_COMM_WORLD);
    // POINT TO POINT SUMMATION!

    clock_t begin1 = clock();

    long long ptop = pTopSum(randNums, numArray, np, pid, status);
    if(pid==0){
        printf("The point to point sum is %d\n", ptop);
   
        clock_t end1 = clock();
        double time_spent1 = (double)(end1 - begin1);

        printf("Time spent in point to point: %f\n", (float)time_spent1/CLOCKS_PER_SEC);
        printf("\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);
    // COLLECTIVE COMMUNICATION SUMMATION! 

    clock_t begin2 = clock();

    long long collSum = collectiveSum(randNums, numArray, np, pid);
    if(pid==0){
        printf("The collective sum is %d\n", collSum);
   
        clock_t end2 = clock();
        double time_spent2 = (double)(end2 - begin2);

        printf("Time spent in collective communication: %f\n", (float)time_spent2/CLOCKS_PER_SEC);
    }
     
    MPI_Finalize();

    return 0;

}   
