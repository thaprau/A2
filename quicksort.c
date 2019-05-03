#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define MASTER 0

int compare (const void * x, const void * y) {
    return (*(int*)x - *(int*)y);
}

int median(int * array, int size) {
    if (size%2 != 0) {
        return array[(size)/2];
    }
    else {
        return (array[(size-1)/2] + array[size/2])/2.0;
    }
}

double mean(int * array, int size) {
    int sum = 0;
    for (int i =0; i<size; i++) {
        sum += array[i];
    }
    return sum/size;
}


int main(int argc, char* argv[]) {

    if (argc!=4){
    printf("\n Usage %s input file, output file, pivot method \n",argv[0]);
    return -1;
    }

    MPI_Status status;
    int rank, size,N;
    int i,istart,istop;
    double pivot;
    int pivot_argument = atoi(argv[3]);
    int* integers=malloc(sizeof(int)*N);
    printf("test1\n");
 
   
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //read the input file 
    if (rank == MASTER) {
        FILE *input=fopen(argv[1],"r");
        fread(&N,sizeof(int),1,input);

        for(int i=1;i<N+1;i++){
            fread(&integers[i],sizeof(int),1,input);
        }
        fclose(input);
    }
    

//divide all integers into p sets of integers
    int p = N/size;       /* Number of intervals per processor */
    istart = rank*p+1;         /* Calculate start and stop indices  */
    if (rank == size-1 ) {
        istop = N;           /* Make sure the last processor      */
    }
    else {
        istop  = (rank + 1)*p;     /* for the local loop                */
    }
    
    /*Creates local array*/
    int sub_array[istop-istart];
    int step = 0;
    for(int i = istart; i<istop; i++) {
        sub_array[step] = integers[i];
        step++; 
    }

    qsort(sub_array, istop-istart, sizeof(int),compare);

    int local_median = median(sub_array, istop-istart);

    if (pivot_argument == 2) {
        int allmedians[size];
        if (rank==MASTER){
            for (int i=1;i<size;i++){
            MPI_Recv(&allmedians[i],1,MPI_INT,i,111,MPI_COMM_WORLD,&status);
            allmedians[0] = local_median; 
            }
        }else{
            MPI_Send(&local_median,1,MPI_INT,0,111,MPI_COMM_WORLD);

        }
    
    pivot = median(allmedians,size);

        if (rank==MASTER){
            for (int i=1;i<size;i++){
                MPI_Send(&pivot, 1, MPI_INT, i, 222, MPI_COMM_WORLD);
            }
        }else{
            MPI_Recv(&pivot,1,MPI_INT,0,222,MPI_COMM_WORLD, &status);

        }




    }

    if (pivot_argument == 3) {
                int allmedians[size];
        if (rank==MASTER){
            for (int i=1;i<size;i++){
            MPI_Recv(&allmedians[i],1,MPI_INT,i,111,MPI_COMM_WORLD,&status);
            allmedians[0] = local_median;
            }
        }else{
            MPI_Send(&local_median,1,MPI_INT,0,111,MPI_COMM_WORLD);

        }

    pivot=mean(allmedians,size);


            if (rank==MASTER){
            for (int i=1;i<size;i++){
            MPI_Send(&pivot,1,MPI_INT,i,222,MPI_COMM_WORLD);
            }
        }else{
            MPI_Recv(&pivot,1,MPI_INT,0,222,MPI_COMM_WORLD, &status);
        }

    }else{
        pivot=local_median;
    }


int smaller[p];
int larger[p];
int smallcounter=0;
int bigcounter=0;

for(int i=0;i<p;i++){
    if (sub_array[i]<pivot){
        smaller[smallcounter]=sub_array[i];
        smallcounter++;
    }else{
        larger[bigcounter]=sub_array[i];
        bigcounter++;
    }
}

    return 0;
}