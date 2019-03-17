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

    pb = fb;
    pFinalMatrix = finalMatrix;


    get_walltime(&ts);
    for (int k=0; k<N; k++) {
        pb = fb;
        for (int j=0; j<N; j++) {
            sum = 0;
            pa = fa + k*N;
            for (int i=0; i<N; i++) {
                sum += (*pa) * (*pb);
                pa++;
                pb++;
            }
            *pFinalMatrix = sum;
            pFinalMatrix ++;
        }
    }
    get_walltime(&te);

    for (int i=0; i<N*N; i++) {
        if (finalMatrix[i] != 6 * N) {
            printf("Wrong result!");
            break;
        }
    }
    printf("%lf\t%lf\n", te-ts, (unsigned long)N*N*N / ((te-ts) * 1e6));

    free(fa);
    free(fb);
    free(finalMatrix);

    return 0;
}


