#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
    if (argc < 2){
        printf(1, "Error: not to pass policy number as an arguemnt\n");
        exit(1);
    }else{
        int n = atoi(argv[1]);
        if (n > 2){
            printf(1, "Error replacing policy, no such a policy number(%d)\n",n);
            exit(1);
        }
        
        int res = policy(n);
        if (res != n){
            printf(1, "Error: system call policy not working well, system call returns %d\n",res);
            exit(1);
        }
        switch (res)
        {
        case 0:
            printf(1, "Policy has been successfully changed to Default Policy\n");
            break;
        case 1:
            printf(1, "Policy has been successfully changed to Priority Policy\n");
            break;
        case 2:
            printf(1, "Policy has been successfully changed to CFS Policy\n");
            break;

        default:
            break;
        }

        exit(0);

    }
    
}
