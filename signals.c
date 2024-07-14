#include "prompt.h"

void Ctrl_C(int signal_number)
{

    pid_t pid = getpid();
    // printf("%d\n",pid);
    // printf("%d\n",foreground);
    if (foreground)
    {
        kill(fg_pid, signal_number);
    }
}

void Ctrl_D()
{
    for (int i = 0; i < bg_count; i++)
    {
        pid_t pid = bg_processes[i].pid;
        kill(pid, 9);
    }
    printf("\n");
    exit(0);
}

void Ctrl_Z(int signal_number)
{
    pid_t foreground_pid = getpgrp(); 

    if (foreground_pid != getpid())
    {
        // printf("%d\n",)
        int chpad = kill(-foreground_pid, signal_number);
        if (chpad == -1)
        {
            perror("Error");
            exit(1);
        }
    }
}

void ping(char *command)
{
    char *args[3];
    int count = 0;
    char *token = strtok(command, " ");
    while (token != NULL)
    {
        args[count] = token;
        token = strtok(NULL, " ");
        count++;
    }
    if (count != 3)
    {
        printf("Invalid command\n");
        return;
    }
    // for(int i =0 ;i<3;i++)
    // {
    //     printf("%s\n",args[i]);
    // }
    int pid = atoi(args[1]);
    int signal_no = atoi(args[2]);
    if (pid <= 0)
    {
        printf("Invalid PID: %s\n", args[1]);
        return;
    }

    if (signal_no < 0)
    {
        printf("Invalid signal number: %s\n", args[2]);
        return;
    }

    int jexx = kill(pid, 0);
    if (jexx == -1)
    {
        printf("No such process found\n"); 
        return;
    }

    signal_no = signal_no % 32;
    int vamc = kill(pid, signal_no);
    if (vamc == -1)
    {
        perror("kill");
        return;
    }
}