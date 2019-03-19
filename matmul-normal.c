#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

struct timeval tp;

void get_walltime(double *wct) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    *wct = (double) (tp.tv_sec + tp.tv_usec/1000000.0);
}

int main() 
{
    float *fa, *fb, *finalMatrix;
    float *pa, *pb, *pFinalMatrix;
    float sum;
    
    double ts, te;

    fa = (float*) malloc(sizeof(float) * N * N);
    if (!fa) {
        exit(-1);
    }
    fb = (float*) malloc(sizeof(float) * N * N);
    if (!fb) {
        free(fa);
        exit(-1);
    }
    finalMatrix = (float*) malloc(sizeof(float) * N * N);
    if (!finalMatrix) {
        free(fa);
        free(fb);
        exit(-1);
    }
    for (int i=0; i<N*N; i++) {
        fa[i] = 2.0;
        fb[i] = 3.0;
        finalMatrix[i] = 0.0;
    }

    //Setting the pointer to show at the begining of the fb array.
    pb = fb;
    //Same goes for pFinalMatrix
    pFinalMatrix = finalMatrix;

    //Here we keep the first tie stamp right befor the calulations
    get_walltime(&ts);
    //This outer-most loop helps us keep the fa array line we are currently working on
    for (int k=0; k<N; k++) {
        //Every time the second loop will have finished, pb pointer
        // will have reached the end of the fb array, so we will have to
        // bring it back to its beggining and start the calculations
        pb = fb;
        //This loop helps us use all lines of the fb array
        for (int j=0; j<N; j++) {
            //We have to initialize the sum sicne it only keeps the resault
            // from one item of the final matrix.
            sum = 0;
            //Here we return the pointer at the start of the current line of the fa array
            pa = fa + k*N;
            //In this for loop the multiplications are being done
            for (int i=0; i<N; i++) {
                sum += (*pa) * (*pb);
                //The pointer have to be moved in order to show the next item in the arrays
                pa++;
                pb++;
            }
            //At this point sum has all the data it needs so we need to pass on to the finalMatrix
            *pFinalMatrix = sum;
            //This pointer has to be moved to the next item on the array
            pFinalMatrix ++;
        }
    }
    //Here we keep another time stamp
    get_walltime(&te);
    //With this for loop we are checking to make sure that all the items in the final array have 
    // been properly calculated
    for (int i=0; i<N*N; i++) {
        if (finalMatrix[i] != 6 * N) {
            printf("Wrong result!");
            break;
        }
    }

    //Printing out the time and estimated Mflops for our program
    printf("%lf\t%lf\n", te-ts, (unsigned long)N*N*N / ((te-ts) * 1e6));

    //Freeing the arrays
    free(fa);
    free(fb);
    free(finalMatrix);

    return 0;
}


