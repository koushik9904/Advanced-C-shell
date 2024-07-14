#include "prompt.h"

int getMostRecentPID()
{
    FILE *loadavg = fopen("/proc/loadavg", "r");
    if (!loadavg)
    {
        perror("fopen");
        exit(1);
    }

    char buffer[256];
    if (fscanf(loadavg, "%*s %*s %*s %*s %s", buffer) == EOF)
    {
        fclose(loadavg);
        perror("fscanf1");
        exit(1);
    }
    fclose(loadavg);
    return atoi(buffer);
}

void Neonate(int time)
{
    if (time <= 0)
    {
        fprintf(stderr, "Invalid time value\n");
        return;
    }
    int ped = fork();
    if (ped == 0)
    {
        while (1)
        {
            int pid = getMostRecentPID();
            if (pid == -1)
            {
                perror("getMostRecentPID");
                return;
            }

            printf("%d\n", pid);
            sleep(time);
        }
    }
    else
    {
        char c;
        if (read(STDIN_FILENO, &c, 1) == 1 && c == 'x')
        {
            kill(ped, SIGKILL);
        }
    }
}