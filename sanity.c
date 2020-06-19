#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"

#define BLOCK_SIZE 512

char buf[8192];
char name[3];
int stdout = 1;

void writetest1(void)
{
    int i, fd, n;
    int nDirect=0, nInDirect=0, nDoubleInDirect=0;

    printf(stdout, "test files test\n");

    fd = open("test", O_CREATE | O_RDWR);
    if (fd < 0)
    {
        printf(stdout, "error: creat test file failed!\n");
        exit();
    }

    for (i = 0; i < NDIRECT; i++)
    {
        ((int *)buf)[0] = i;
        if (write(fd, buf, BLOCK_SIZE) != BLOCK_SIZE)
        {
            printf(stdout, "error: write big file failed\n", i);
            exit();
        }

        nDirect+= BLOCK_SIZE;
        
    }

    printf(1,"Finished writing %dKB (direct)\n",nDirect/1000);
    

    for (i = NDIRECT; i < OLD_MAXFILE; i++)
    {
        ((int *)buf)[0] = i;
        if (write(fd, buf, BLOCK_SIZE) != BLOCK_SIZE)
        {
            printf(stdout, "error: write big file failed\n", i);
            exit();
        }
        
        nInDirect += BLOCK_SIZE;
        
    }

    printf(1,"Finished writing %dKB (single indirect)\n",nInDirect/1000 + nDirect/1000);

    printf(1,"writing %d times 1 byte\n",MAXFILE-OLD_MAXFILE);
    for (i = OLD_MAXFILE; i < MAXFILE; i++)
    {
        
            printf(1,"a\n");

        ((int *)buf)[0] = i;
        if (write(fd, buf, 1) != 1)
        {
            printf(stdout, "error: write big file failed\n", i);
            exit();
        }

        nDoubleInDirect += BLOCK_SIZE;
    }

    printf(1,"Finished writing %dKB\n",nDoubleInDirect/1000);


    close(fd);

    fd = open("test", O_RDONLY);
    if (fd < 0)
    {
        printf(stdout, "error: open test failed!\n");
        exit();
    }

    n = 0;
    for (;;)
    {
        i = read(fd, buf, BLOCK_SIZE);
        if (i == 0)
        {
            if (n == MAXFILE - 1)
            {
                printf(stdout, "read only %d blocks from test", n);
                exit();
            }
            break;
        }
        else if (i != BLOCK_SIZE)
        {
            printf(stdout, "read failed %d\n", i);
            exit();
        }
        if (((int *)buf)[0] != n)
        {
            printf(stdout, "read content of block %d is %d\n",
                   n, ((int *)buf)[0]);
            exit();
        }
        n++;
    }
    close(fd);
    if (unlink("test") < 0)
    {
        printf(stdout, "unlink test failed\n");
        exit();
    }

    printf(stdout, "test files ok\n");
}