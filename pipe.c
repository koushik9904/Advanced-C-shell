#include "prompt.h"



void pipes(char *command, char *prev_wd, char *home_for_shell, char *cwd, char *path_to_historyfile)
{
    char **args = (char**)malloc(sizeof(char*)*20);
    int count = 0;
    char *token = strtok(command, "|");
    
    while (token != NULL)
    {   
        args[count] = token;
        // printf("%s\n",args[count]);
        count++;
        token = strtok(NULL, "|");
    }
    int file_des[2];
    int fd= 0;
    pipe(file_des);
    // printf()
    pid_t pid;
    // printf("%s\n",args[0]);
    for (int i = 0; i < count; i++)
    {
        
        pid = fork();
        // printf("%d\n",pid);
        if (pid == 0)
        {   
            int k = dup2(fd,0);
            if (args[i + 1] != NULL)
            {
                dup2(file_des[1], 1);
            }
            int kk = close(file_des[0]);
            // printf("%s\n",args[i]);
            execute_commands(prev_wd, home_for_shell, cwd, path_to_historyfile,args[i]);
            exit(0);
        }
        else if(pid > 0)
        {
            // printf("Hehe\n");
            wait(NULL);
            close(file_des[1]);
            fd = file_des[0];  
        }
        else 
        {
            return;
        }
    }
}