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
#define BUFSIZE 8192

#define KB 1024
#define MB (KB * KB)


char buf[BUFSIZE];
char name[3];
int stdout = 1;

void writetest1(void)
{
    int i, fd;
    int nDirect=0, nInDirect=0, nDoubleInDirect=0, nRead;

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

    printf(1,"Finished writing %dKB (direct)\n",nDirect/KB);
    

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

    printf(1,"Finished writing %dKB (single indirect)\n",nInDirect/KB + nDirect/KB);

    for (i = OLD_MAXFILE; i < OLD_MAXFILE + MB/BLOCK_SIZE; i++)
    {

        ((int *)buf)[0] = i;
        if (write(fd, buf, BLOCK_SIZE) != BLOCK_SIZE)
        {
            printf(stdout, "error: write big file failed\n", i);
            exit();
        }

        nDoubleInDirect += BLOCK_SIZE;
    }

    printf(1,"Finished writing %dMB\n",nDoubleInDirect/MB);

    close(fd);

    fd = open("test", O_RDONLY);

    printf(1,"read data to check writing\n");

    memset(buf,0,BUFSIZE);
    for (i = 0; i < OLD_MAXFILE + MB/BLOCK_SIZE; i++){
        nRead = read(fd,buf,BLOCK_SIZE);
        if(nRead == -1){
            printf(1,"failed to read block number %d\n",i);
            exit();
        }
        int data = ((int *)buf)[0];
        if(data != i){
            printf(1,"wrong data in block number %d. read:%d\n",i,data);
            exit();
        }
    }
    

    close(fd);

    exit();

}