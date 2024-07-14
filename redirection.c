#include "prompt.h"

void redirect(char *command, char *input, char *prev_wd, char *home_for_shell, char *cwd, char *path_to_historyfile)
{

    int d_stdout = dup(STDOUT_FILENO);
    int d_stdin = dup(STDIN_FILENO);
    int In_file = 0, output = 0, status;

    char *cmd = (char *)malloc(sizeof(char) * 1024);

    if (strstr(command, "<") != NULL)
    {
        In_file = 1;
    }
    if (strstr(command, ">>") != NULL)
    {
        output = 2;
    }
    else if (strstr(command, ">") != NULL)
    {
        output = 1;
    }

    char *p;
    char *out[2];
    out[0] = command;
    char *output_file;

    if (output)
    {
        out[0] = strtok(command, ">");
        strcpy(cmd, out[0]);
        out[1] = strtok(NULL, ">");
        output_file = strtok(out[1], " \n");
    }
    char *cmd1 = strdup(cmd);
    p = out[0];
    char *in[2];
    char *input_file;
    if (In_file)
    {
        int flag = 0;
        in[0] = strtok(p, "<");
        strcpy(cmd, in[0]);
        in[1] = strtok(NULL, "<");
        input_file = strtok(in[1], " \n");
    }
    pid_t pid = fork();
    if (pid < 0)
    {
        return;
    }
    if (pid == 0)
    {
        if (In_file)
        {
            int fd = open(input_file, O_RDONLY);
            if (fd < 0)
            {
                return;
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        if (output)
        {
            int fd;
            if (output == 1)
            {
                // printf("HLO\n");
                fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            }
            else if (output == 2)
            {
                fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            }
            if (fd < 0)
            {
                return;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        execute_commands(prev_wd, home_for_shell, cwd, path_to_historyfile, cmd);
        dup2(d_stdin, STDIN_FILENO);
        close(d_stdin);
        dup2(d_stdout, STDOUT_FILENO);
        close(d_stdout);
    }
    else
        wait(NULL);
}