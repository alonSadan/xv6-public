#include "types.h"
#include "user.h"

//void* fatherStats;
struct stats stats;
int status, pid, retime, rtime, stime, ps_priority;
int sums[3][3];
long long accumulator;
double runTimeRatio, cfs_priority;
void procTester(int priority, int cfs_priority);
void UpdateSumsAndPrintProcStats(int procType, int pid);
int round(double x);
int priorityArr[] = {10, 5, 1};
int cfs_priorityArr[] = {3, 2, 1};
char *procTypeArray[] = {"CPU-Bound", "Short_Tasks", "I/O-Bound"};
char *decay[] = {"0.75", "1.0", "1.25"};

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf(1, "Usage: sanity [n]\n");
		exit(1);
	}
	int i;
	int numOfProc;
	int j = 0;
	int rand1 = 5;
	int rand2 = 13;

	printf(1, "PROC_TYPE\tPID\tRETIME\tRTIME\tSTIME\tTURNARROUND\tPS_PRIORITY\tCFS_PRIORITY\tRUN_TIME_RATIO\t_ACCUMULATOR\n");

	for (i = 0; i < 3; i++) //sums array initialization
	{
		for (j = 0; j < 3; j++)
		{
			sums[i][j] = 0.0;
		}
	}

	numOfProc = atoi(argv[1]); //get argument

	for (i = 0; i < numOfProc; i++)
	{
		rand1 += rand2 * 7;
		rand2 += rand1 * 13;

		//j = i % 3;
		pid = fork();
		if (pid == 0)
		{ //child
			// it is possible to modify rand1/rand2 and control the prioryties for the different process type.
			procTester(priorityArr[rand1 % 3], cfs_priorityArr[rand2 % 3]);
			exit(0);
		}

		continue; // father continues to spawn the next child
	}
	for (i = 0; i < numOfProc; i++)
	{
		pid = wait2(&retime, &rtime, &stime, &ps_priority, &cfs_priority, &runTimeRatio, &accumulator);
		j = (pid - 4) % 3;
		UpdateSumsAndPrintProcStats(j, pid);
	}
	//maybe print totlas

	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			sums[i][j] *= 100;
			sums[i][j] /= numOfProc;
		}
	}
	printf(1, "\n\nCPU bound:\nAverage ready time: %d\nAverage running time: %d\nAverage sleeping time: %d\nAverage turnaround time: %d\n\n\n", sums[0][0], sums[0][1], sums[0][2], sums[0][0] + sums[0][1] + sums[0][2]);
	printf(1, "CPU-S bound:\nAverage ready time: %d\nAverage running time: %d\nAverage sleeping time: %d\nAverage turnaround time: %d\n\n\n", sums[1][0], sums[1][1], sums[1][2], sums[1][0] + sums[1][1] + sums[1][2]);
	printf(1, "I/O bound:\nAverage ready time: %d\nAverage running time: %d\nAverage sleeping time: %d\nAverage turnaround time: %d\n\n\n", sums[2][0], sums[2][1], sums[2][2], sums[2][0] + sums[2][1] + sums[2][2]);
	exit(0);
}

void procTester(int priority, int cfs_priority)
{
	int l;
	int k;
	int p;
	if ((set_ps_priority(priority)) < 0)
	{
		printf(2, "error in set_ps_priority\n");
		exit(1);
	}
	if ((set_cfs_priority(cfs_priority)) < 0)
	{
		printf(2, "error in set_cfs_priority\n");
		exit(1);
	}

	p = getpid();
	l = (p - 4) % 3; // ensures independence from the first son's pid when gathering the results in the second part of the program

	switch (l)
	{
	case 0: //CPU‐bound process (CPU):
		for (double z = 0; z < 10000.0; z += 0.1)
		{
			double x = x + 3.14 * 89.64; // useless calculations to consume CPU time
		}
		break;
	case 1: //short tasks based CPU‐bound process (S‐CPU):
		for (k = 0; k < 100; k++)
		{
			for (int q = 0; q < 1000000; q++)
			{
			}
			yield();
		}
		break;
	case 2: // simulate I/O bound process (IO)
		for (k = 0; k < 100; k++)
		{
			sleep(1);
		}
		break;
	}

	if (proc_info(&stats) < 0)
	{
		exit(1);
	}
}

void UpdateSumsAndPrintProcStats(int procType, int pid)
{
	int cfs_index = round(cfs_priority);
	sums[procType][0] += retime;
	sums[procType][1] += rtime;
	sums[procType][2] += stime;

	runTimeRatio *= 100;

	printf(1, "%s\t%d\t%d\t%d\t%d\t%d\t\t%d\t\t%s\t\t%d\t\t%d\n",
		   procTypeArray[procType], pid, retime, rtime, stime, retime + rtime + stime,
		   ps_priority, decay[cfs_index], (int)runTimeRatio, accumulator);
}

int round(double x)
{
	if (x > 1)
	{
		return 2;
	}
	else if (x < 1)
	{
		return 0;
	}
	return 1;
}