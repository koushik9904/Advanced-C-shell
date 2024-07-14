#include "prompt.h"

void fg(int pid)
{
    int jexx = kill(pid, 0);
    if (jexx == -1)
    {
        printf("No such process found\n"); 
        return;
    }
    tcsetpgrp(STDIN_FILENO, pid);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);

    kill(pid, SIGCONT);
    int status;
    waitpid(-1, &status, WUNTRACED);

    tcsetpgrp(STDIN_FILENO, getpgrp());
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
}