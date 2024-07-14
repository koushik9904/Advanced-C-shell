#include "prompt.h"


void prompt(char *home_for_shell, char *cwd, char *prev_wd, char *username, char *sys_name, long int flag, char *prev_input)
{
    // Do not hardcode the prmopt
    getcwd(cwd, sizeof(cwd));
    // if (strcmp(cwd, home_for_shell) == 0)
    // {
    //     printf("%s@%s:~>", username, sys_name);
    // }
    // else
    // {
    //     printf("%s@%s:%s>", username, sys_name, cwd);
    // }
    
    // printf()
    if (flag == 0)
    {
        if (strcmp(cwd, home_for_shell) == 0)
        {
            printf("\x1B[32;1m%s@%s:\x1B[0m\033[1;34m~>\x1B[0m", username, sys_name);
        }
        else
        {
            char *CWD = (char *)malloc(sizeof(char) * 1024);
            int len = strlen(home_for_shell);
            if (strncmp(home_for_shell, cwd, len) == 0)
            {
                strcpy(CWD, cwd);
                CWD[len - 1] = '~';
                CWD = CWD + len - 1;
                printf("\x1B[32;1m%s@%s:\x1B[0m\033[1;34m%s>\x1B[0m", username, sys_name, CWD);
            }
            else
                printf("\x1B[32;1m%s@%s:\x1B[0m\033[1;34m%s>\x1B[0m", username, sys_name, cwd);

            // free(CWD);
        }
    }
    else
    {   
        char *token = strtok(prev_input," \n");
        if (strcmp(cwd, home_for_shell) == 0)
        {
            printf("\x1B[32;1m%s@%s:\x1B[0m\033[1;34m~%s : %lds>\x1B[0m", username, sys_name,token,flag);
            // flag =0;
        }
        else
        {
            char *CWD = (char *)malloc(sizeof(char) * 1024);
            int len = strlen(home_for_shell);
            if (strncmp(home_for_shell, cwd, len) == 0)
            {
                strcpy(CWD, cwd);
                CWD[len - 1] = '~';
                CWD = CWD + len - 1;
                printf("\x1B[32;1m%s@%s:\x1B[0m\033[1;34m%s %s : %lds>\x1B[0m", username, sys_name, CWD,token,flag);
            }
            else
                printf("\x1B[32;1m%s@%s:\x1B[0m\033[1;34m%s %s : %lds>\x1B[0m", username, sys_name, cwd,token,flag);

            // free(CWD);
        }
        // flag =0;
    }
}
