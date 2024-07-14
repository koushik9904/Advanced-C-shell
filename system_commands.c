#include "prompt.h"


void system_commands(char *Command,int background)
{
    char *command = strtok(Command, "\n");
    // int background = 0;
    pid_t child_pid = fork();

    if (background != 1)
    {
        

        if (child_pid == 0)
        {
           
            char *args[18]; 
            // printf("%s", command);
            char *token = strtok(command, " ");
            int i = 0;
            while (token != NULL)
            {
                args[i] = token;
                // printf("%s", token);
                token = strtok(NULL, " ");
                i++;
            }
            args[i] = NULL;

            // printf("DS");
            
            // printf("%d\n",fg_pid);
            int chvk = execvp(args[0], args);
            // printf("%d\n",chvk);
            if(chvk == -1)
            {
                printf("ERROR : '%s' is not valid command\n",args[0]);
            }
        }
        else if (child_pid > 0)
        {
            int status;
            foreground = 1;
            time_t start_time = time(NULL);
            fg_pid = child_pid;
            waitpid(child_pid, &status, 0);
            foreground = 0;
            time_t end_time = time(NULL);

            if (end_time - start_time > 2)
            {
                flag = end_time - start_time;
            }
        }
        else
        {
            perror("Fork failed");
            // exit(1);
        }
        
    }
    else
    {
        if (child_pid == 0)
        {
           
            char *args[32]; 
            // printf("%s", command);
            char *token = strtok(command, " ");
            int i = 0;
            while (token != NULL)
            {
                args[i] = token;
                // printf("%s", token);
                token = strtok(NULL, " ");
                i++;
            }
            args[i] = NULL;

            // printf("DS");
            execvp(args[0], args);
            // perror("Execution failed");
            // exit(1);
        }
        else if (child_pid > 0)
        {
            printf("%d\n", child_pid);
            bg_processes[bg_count].pid = child_pid;
            strcpy(bg_processes[bg_count].command, command);
            bg_processes[bg_count].running = 1;
            bg_count++;

            // bg_processes_list[bg_count_list].pid = child_pid;
            // strcpy(bg_processes_list[bg_count_list].command, command);
            // bg_count_list++;
        }
        else
        {
            perror("Fork failed");
            // exit(1);
        }
    }
}
