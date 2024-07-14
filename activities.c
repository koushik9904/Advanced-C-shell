#include "prompt.h"

void swap(struct BackgroundProcess *a, struct BackgroundProcess *b)
{
    struct BackgroundProcess temp = *a;
    *a = *b;
    *b = temp;
}

void activity()
{
    // fclose(status_fp);
    // char process_state = process_status[7];
    struct BackgroundProcess bg_cpy[100];
    // printf("%s\n",bg_processes[0].command);
    for (int i = 0; i < bg_count; i++)
    {
        strcpy(bg_cpy[i].command, bg_processes[i].command);
        bg_cpy[i].pid = bg_processes[i].pid;
    }
    // for (int i = 0; i < bg_count; i++)
    // {
    //     printf("%d %s\n", bg_cpy[i].pid, bg_cpy[i].command);
    // }
    int min_idx;

    for (int i = 0; i < bg_count - 1; i++)
    {
        min_idx = i;
        for (int j = i + 1; j < bg_count; j++)
        {
            if (bg_cpy[j].pid < bg_cpy[min_idx].pid)
            {
                min_idx = j;
            }
        }
        if (min_idx != i)
        {
            swap(&bg_cpy[i], &bg_cpy[min_idx]);
        }
    }
    // for (int i = 0; i < bg_count; i++)
    // {
    //     printf("%d %s\n", bg_cpy[i].pid, bg_cpy[i].command);
    // }
    for (int i = 0; i < bg_count; i++)
    {
        // printf("%d\n",bg_cpy[i].pid);
        char status_file[1024];
        snprintf(status_file, 1024, "/proc/%d/status", bg_cpy[i].pid);
        FILE *status_fp = fopen(status_file, "r");
        if (status_fp == NULL)
        {
            // printf("hgd\n");
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
        if (process_state != 'T')
            printf("%d : %s - Running\n", bg_cpy[i].pid, bg_cpy[i].command);
        else
            printf("%d : %s - Stopped\n", bg_cpy[i].pid, bg_cpy[i].command);

        // if (bg_cpy[i].status == 1)
        //     printf("%d : %s - Running", bg_cpy[i].pid, bg_cpy[i].command);
        // else
        //     printf("%d : %s - Stopped", bg_cpy[i].pid, bg_cpy[i].command);
    }
}