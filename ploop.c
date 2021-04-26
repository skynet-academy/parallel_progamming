#include<stdio.h>
#include<omp.h>


void subdomain(float *x, int istart, int ipoints){
    int i;
    for(i=0; i < ipoints; i++)
        x[istart + i] = 123.456;
}

void sub(float *x, int npoints){
    int iam, nt, ipoints, istart;
#pragma omp parallel default(shared) private(iam, nt, ipoints, istart)
    {
        iam = omp_get_thread_num();
        printf("This is the thread nro: %d \n", iam);
        nt = omp_get_num_threads();
        ipoints = npoints / nt;
        printf("the value of ipoints is : %2d \n", ipoints);
        istart = iam * ipoints;
        printf("the value of istart is : %2d \n\n", istart);
        if(iam==nt-1)
            ipoints = npoints - istart;
        subdomain(x , istart, ipoints);
    }
}


int main(){
    float array[10000];
    sub(array, 10000);
    return 0;
}
