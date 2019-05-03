#include <stdlib.h>
#include <stdio.h>


int main(int argc, char* argv[]){

    int number_size = 100;
    int N = atoi(argv[1]);
    FILE * fp = fopen("INPUT", "w");


    char space = ' ';
    fwrite(&N, 1, sizeof(int), fp);
    fwrite(&space, 1, sizeof(char), fp);

    int number;
    

    for(int i = 0; i<N; i++) {

        int number = rand() % number_size;

        fwrite(&number, 1, sizeof(int), fp);
        fwrite(&space, 1, sizeof(char), fp);

    }

    fclose(fp);


}