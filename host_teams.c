#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<omp.h>
#define N 1000

int main()
{
    int nteams_required=2, max_thrds, tm_id;
    float sp_x[N], sp_y[N], sp_a=0.0001e0;
    double dp_x[N], dp_y[N], dp_a=0.0001e0;
    #pragma omp teams num_teams(nteams_required) thread_limit(max_thrds) private(tm_id)
    {
        tm_id = omp_get_team_num();
        if(omp_get_num_teams()!=2){
            printf("error: Insufficient teams on host, 2 required\n");
            exit(0);
        }
        if(tm_id == 0)
        {
            #pragma omp parallel
            {
                #pragma omp for
                for(int i=0; i < N; i++)
                {
                    sp_x[i] = i*0.0001;
                    sp_y[i] = i;
                }
                #pragma omp for simd simdlen(8)
                for(int i=0; i< N ; i++)
                {
                    sp_x[i] = sp_a * sp_x[i] + sp_y[i];
                }
            }
        }
        if(tm_id=1)
        {
            #pragma omp parallel
            {
                #pragma omp for
                for(int i=0; i< N; i++)
                {
                    dp_x[i] = i*0.0001;
                    dp_y[i]= i;
                }
                #pragma omp for simd simdlen(4)
                for(int i=0; i<N; i++)
                {
                    dp_x[i] = dp_a * dp_x[i] + dp_y[i];
                }
            }
        }
    }
    printf("i=%d sp|dp %f %f \n", N-1, sp_x[N-1], dp_x[N-1]);
    printf("i=%d sp|dp %f %f \n", N/2, sp_x[N/2], dp_x[N/2]);
    return 0;
}
