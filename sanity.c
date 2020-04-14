#include "types.h"
#include "stat.h"
#include "user.h"

struct perf performance;
int status,pid;

void procTester(int priority,int cfs_priority,long long i){
    if((set_ps_priority(priority)) < 0){
        printf(2,"error in set_ps_priority");
        exit(1);
    }
    if((set_cfs_priority(cfs_priority)) < 0){
        printf(2,"error in set_cfs_priority");
        exit(1);
    }

    for(double j = 0;j < i; j+=0.01){
        double x =  x + 3.14 * 89.64;    
    }

    // int dummy = 0;
    // while(i--){
    //     dummy+=i;
    // }  
   
    //sleep(1);

    if(proc_info(&performance) < 0) {
        exit(1);
    }else{
        printf(1, "%d\t%d\t\t%d\t%d\t%d\n",getpid(),
            performance.ps_priority,
            performance.stime,
            performance.retime,
            performance.rtime);
    }
}

int main (int argc, char **argv){
    
    int processNum=3;
    int priorityArr[] = {10,5,1};
    int cfs_priorityArr[] = {3,2,1};
    long long iArr[] = {10000,10000,10000};

    printf(1,"PID\tPS_PRIORITY\tSTIME\tRETIME\tRTIME\n");

    for (int kid = 0; kid < processNum; kid++) {
        //Child process
        if ((pid = fork()) == 0){
            procTester(priorityArr[kid],cfs_priorityArr[kid],iArr[kid]);
            exit(0);
            //sleep(200);
        }
    }
    for (int kid = 0; kid < processNum; ++kid) {
        wait(&status); // kids could be ready in any order
        //sleep(20);
    }  
        
    exit(0);
}