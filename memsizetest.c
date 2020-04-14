#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    printf(1, "The processs is using %d\n",(uint)memsize());
    //int *x = (int*) malloc(50*sizeof(int));
    char* x = sbrk(sizeof(char)*2000);  
    printf(1, "Allocating more memory\n");
    printf(1, "The processs is using %d\n",memsize());
    free(x);
    printf(1, "Freeing memory\n");
    printf(1, "The processs is using %d\n",memsize());
    exit(0);
}
