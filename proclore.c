#include "prompt.h"



void get_process_info(int pid, char *home_for_shell, char *cwd)
{
    char status_file[1024];
    char executable_file[1024];

    snprintf(status_file, sizeof(status_file), STATUS_PATH, pid);
    snprintf(executable_file, sizeof(executable_file), EXECUTABLE_PATH, pid);

    // Read process status from /proc/pid/status
    FILE *status_fp = fopen(status_file, "r");
    if (status_fp == NULL)
    {
        perror("Error opening status file");
        return;
    }
    
    char process_status[32];

    while (fgets(process_status, sizeof(process_status), status_fp))
    {
        if (strncmp(process_status, "State:", 6) == 0)
        {
            break;
        }
    }

    fclose(status_fp);
    char process_state = process_status[7];
    // printf("%s\n",process_status);
    char process_plus = process_status[8];

    FILE *mem_fp = fopen(status_file, "r");
    if (mem_fp == NULL)
    {
        perror("Error opening status file");
        return;
    }
    char virtual_memory[64];

    while (fgets(virtual_memory, sizeof(virtual_memory), mem_fp))
    {
        if (strncmp(virtual_memory, "VmSize:", 7) == 0)
        {
            break;
        }
    }

    fclose(mem_fp);
    char executable_path[1024];

    ssize_t EXECUTABLE_PATH_length = readlink(executable_file, executable_path, sizeof(executable_path) - 1);
    if (EXECUTABLE_PATH_length == -1)
    {
        perror("Error reading executable path");
        // return;
    }
    executable_path[EXECUTABLE_PATH_length] = '\0';

    pid_t process_group = getpgid(pid);

    // Print the process information
    printf("pid : %d\n", pid);
    if (process_plus == '+')
        printf("process status : %c+\n", process_state);
    else
        printf("process status : %c\n", process_state);
    printf("Process Group : %d\n", process_group);
    printf("Virtual memory : %s", virtual_memory + 11);
    char *CWD = (char *)malloc(sizeof(char) * 1024);
    int len = strlen(home_for_shell);
    if (strncmp(home_for_shell, cwd, len) == 0)
    {
        strcpy(CWD, executable_path);
        // CWD[len - 1] = '~';
        // CWD = CWD + len - 1;
        printf("executable path : %s\n",executable_path);
    }
    else
        printf("executable path : %s\n",executable_path);

}
