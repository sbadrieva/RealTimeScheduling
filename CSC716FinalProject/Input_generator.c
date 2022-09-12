#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdio.h>

double ran_expo(double lambda){
    double u;
    u = rand() / (RAND_MAX + 1.0);
    return -log(1- u) / lambda;
}

int main(void)
{
    int i,arrival,execution_time,deadline,period,num_process=5,process_switch=0;
    srand((unsigned)time(NULL));
    
    printf("%d %d \n", num_process, process_switch);
    for (i=0; i<5; i++)
    {
        
        
        // Process number 
        printf("%d ", i);
        arrival = ran_expo(0.05);
        printf("%d ", arrival);
        
        // rand() % 70 generate random number between 0 and 70
        // plus 30, random numbers are generated between 30-100
        execution_time = 30 + rand() % 70;  
        printf("%d ", execution_time);
        
        // deadlines, adds random number between 0 to 500 to execution time
        deadline = execution_time + rand()%500;
        printf("%d ", deadline);
        
        // deadlines, adds random number between 0 to 1000 to deadline
        period = deadline + rand()%1000;
        printf("%d ", period);
        
        printf("\n");
    }
    return 0;
}