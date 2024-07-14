#include "prompt.h"

void bg(int pid)
{
    for (int i = 0; i < bg_count; i++)
    {
        if (bg_processes[i].pid == pid)
        {
            // Change the state of the process to running by sending SIGCONT
            int sudhan = kill(pid, SIGCONT) == -1;
            if (sudhan == -1)
            {
                perror("kill");
                exit(1);
            }
            bg_processes[i].running = 1; 
            return;
        }
    }
    printf("No such process found\n");
    
}