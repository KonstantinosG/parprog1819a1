#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <emmintrin.h>

struct timeval tp;

void get_walltime(double *wct) {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    *wct = (double) (tp.tv_sec + tp.tv_usec/1000000.0);
}

int main() {
    //multArray keeps the resault of the two row's multiplication
    float *a, *b, *finalMatrix, *pFinalMatrix, *multArray, sum;
    __m128 *vfa, *vfb, *vmultArray;

    //Time keeping variables
    double ts, te;
    //On this version of our multiplication we are inclined to use posix_memalign
    // because of the way we want to manipulate the memory. We are using a 16 alignment
    // so that when the _mm_mul_ps process tries to use 4 elements from the arrays we 
    // pass as arguments, it succeds every time. To put it in simple terms the memory
    // we are trying to allocate needs to be a multiple of 16 in order for our program to
    // run properly. This number depends on the architecture of the processor.
    int i = posix_memalign((void**) &a, 16, N*N*sizeof(float));
    if (i != 0) {
        printf("Error allocating memory for a!\n");
        exit(-1);
    }
    int j = posix_memalign((void**) &b, 16, N*N*sizeof(float));
    if (j != 0) {
        free(a);
        printf("Error allocating memory for b!\n");
        exit(-1);
    }
    int k = posix_memalign((void**) &finalMatrix, 16, N*N*sizeof(float));
    if (k != 0) {
        free(a);
        free(b);
        printf("Error allocating memory for finalMatrix!\n");
        exit(-1);
    }
    int m = posix_memalign((void**) &multArray, 16, 4*sizeof(float));
    if (m != 0) {
        free(a);
        free(b);
        free(finalMatrix);
        printf("Error allocating memory for multArray!\n");
        exit(-1);
    }

    //Initializing the arrays
    for(int i = 0; i < N*N; i++) {
        a[i] = 2.0;
        b[i] = 3.0;
        finalMatrix[i] = 0.0;
    }

    //vfa and vfb pointers will be set to show at fa and fb arrays respectively
    vfa = (__m128*) a;
    vfb = (__m128*) b;
    //vmultArray is used to show to the multArray. multArray was created 
    // so that we can access float numbers individually and use them 
    // for the addition(sum).
    vmultArray = (__m128*) multArray;
    
    //pFinalMatrix pointer is being used to add the new values to the array. 
    // Using just one pointer for this array it would make it hard to make it 
    // point at the beginning of the array when the whole process was done. We 
    // need to the beginning of the array in order to check if all items have 
    // been calculated properly.
    pFinalMatrix = finalMatrix;
    
    get_walltime(&ts);
    for(int k = 0; k < N; k++) {
        //For every new k the vfb pointer will have reached to the last item of the b array, 
        // so we need to bring it back to the beginning and start multiplying the items from 
        // the next line of a array.
        vfb = (__m128*) b;
        for(int j = 0; j < N; j++) {
            //Bringing back the vfa pointer to the beginning of the current line is prerequisite 
            // for multiplying the same line from array a with all the lines from array b. 
            vfa = (__m128*)(a + k*N);
            //When the next loop finishes we will have kept all the information gathered inside 
            // the sum from the multiplications and the additions.
            sum = 0;
            for(int n = 0; n < N; n+=4) {
                //With this line four items from the fa array are being multiplies with the equivalent
                // four from the fb array using the SSE2 command shown.
                *vmultArray = _mm_mul_ps(*vfa, *vfb);
                //Four numbers from the multArray are being added into sum.
                sum += multArray[0] + multArray[1] + multArray[2] + multArray[3];
                
                //Now both vfa and vfb will need to point to the next four items from a and b arrays 
                // respectively since we are done with the previous ones.
                vfa++;
                vfb++;
            }
            //Now that the inner most loop is done, we have calculated a new item from the finalMatrix,
            // so we add it to the item the pFinalMatrix pointer shows at.
            *pFinalMatrix = sum;
            //And we move on to the next item of the array.
            pFinalMatrix++;
        }
    }
    get_walltime(&te);

    printf("%lf\t%lf\n", te-ts, (unsigned long)N*N*N / ((te-ts) * 1e6));

    //Checking if the multiplications were done properly.
    for(int i = 0; i < N*N; i++) {
        if (finalMatrix[i] != a[i] * b[i] * N) {
            printf("Error in Multiplication\n");
        }
    }

    //We are finally done and we are freeing the memory we used.
    free(a);
    free(b);
    free(finalMatrix);
    free(multArray);
    
    return 0;
}


