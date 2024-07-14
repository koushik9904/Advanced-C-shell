#include "prompt.h"


#define MAX_COMMANDS 5
#define MAX_COMMAND_LENGTH 4096

void execute_commands(char *prev_wd, char *home_for_shell, char *cwd, char *path_to_historyfile, char *input)
{
    char *delimiter = " \n";
    char command[1024];
    strcpy(command, input);
    // printf("%s\n", input);
    char *cmd = strtok(input, delimiter);

    getcwd(cwd, sizeof(cwd));

    if (strstr(command, "|"))
    {
        pipes(command, prev_wd, home_for_shell, cwd, path_to_historyfile);
    }
    else if (strstr(command, "<") || strstr(command, ">") || strstr(command, ">>"))
    {
        // printf("Here!!\n");
        redirect(command, input, prev_wd, home_for_shell, cwd, path_to_historyfile);
    }
    else if (strcmp(cmd, "warp") == 0)
    {
        cmd = strtok(NULL, delimiter);
        // printf("%s",cmd);
        while (cmd != NULL)
        {
            getcwd(cwd, sizeof(cwd));
            // printf("%s\n",cmd);
            warp_cmd(cmd, input, prev_wd, home_for_shell);
            strcpy(prev_wd, cwd);
            cmd = strtok(NULL, delimiter);
        }
    }
    else if (strcmp(cmd, "peek") == 0)
    {
        cmd = strtok(NULL, delimiter);
        int flag_l = 0;
        int flag_a = 0;
        if (cmd != NULL)
        {
            while (cmd != NULL && cmd[0] == '-')
            {

                if (strcmp(cmd, "-l") == 0)
                {
                    flag_l = 1;
                }
                else if (strcmp(cmd, "-a") == 0)
                {
                    flag_a = 1;
                }
                else if (strcmp(cmd, "-al") == 0 || strcmp(cmd, "-la") == 0)
                {
                    flag_a = 1;
                    flag_l = 1;
                }
                // char *cmd1 = cmd;
                cmd = strtok(NULL, delimiter);
                // printf("%s\n",cmd);
            }
        }
        // printf("%s\n",cmd);
        // if(cmd)
        //     printf("%s\n",cmd);
        // printf("%s\n",cwd);
        if (cmd == NULL)
            peek_cmd(flag_l, flag_a, cwd);
        else if (cmd[0] == '/')
            peek_cmd(flag_l, flag_a, cmd);
        else
        {
            char *path = warp_for_peek(cmd, input, prev_wd, home_for_shell);
            peek_cmd(flag_l, flag_a, path);
        }
    }
    else if (strcmp(cmd, "pastevents") == 0)
    {
        // printf("HHi\n");
        // Handle pastevents command
        cmd = strtok(NULL, delimiter);
        // printf("%s\n",cmd);
        if (cmd == NULL)
        {
            // printf("Hi\n");
            print_history();
        }
        else if (strcmp(cmd, "purge") == 0)
        {
            // printf("cleared\n");
            clear_history(path_to_historyfile);
        }
        else if (strcmp(cmd, "execute") == 0)
        {
            cmd = strtok(NULL, delimiter);
            int num = atoi(cmd);
            cmd = pastevent_execute(num);
            // printf("%s\n",cmd);
            execute_commands(prev_wd, home_for_shell, cwd, path_to_historyfile, cmd);
            // input_requirements(cmd, prev_wd, home_for_shell, cwd, path_to_historyfile);
        }
    }
    else if (strcmp(cmd, "proclore") == 0)
    {
        // Handle proclore command
        int pid;
        cmd = strtok(NULL, delimiter);
        if (cmd == NULL)
        {
            pid = getpid();
            get_process_info(pid, home_for_shell, cwd);
        }
        else
        {
            pid = atoi(cmd);
            get_process_info(pid, home_for_shell, cwd);
        }
    }
    else if (strcmp(cmd, "seek") == 0)
    {
        seek_cmd(command);
    }
    else if (strcmp(cmd, "activities") == 0)
    {
        // printf("nobi\n");
        activity();
        // return;
    }
    else if (strcmp(cmd, "ping") == 0)
    {
        ping(command);
    }
    else if(strcmp(cmd,"bg")==0)
    {
        cmd = strtok(NULL," ");
        int num = atoi(cmd);
        bg(num);
    }
    else if(strcmp(cmd,"fg")==0)
    {
        cmd = strtok(NULL," ");
        int num = atoi(cmd);
        fg(num);
    }
    else if(strcmp(cmd,"iMan")==0)
    {   

        cmd = strtok(NULL," \n");
        // printf("%s\n",cmd);
        iMan_cmd(cmd);
    }
    else if(strcmp(cmd,"neonate")==0)
    {
        cmd = strtok(NULL," \n");
        cmd = strtok(NULL," \n");
        int time = atoi(cmd);
        enableRawMode();
        Neonate(time);
        disableRawMode();
    }
    else
    {
        // printf("%s\n",command);
        if (strstr(command, "&"))
        {
            char *copy = strdup(command);
            char *bg_command = strtok(copy, "&\n");
            char *fg_command = strtok(NULL, "&\n");
            if (bg_command != NULL)
                system_commands(bg_command, 1);
            // printf("hellos\n");
            // printf("%s\n",fg_command);
            if (fg_command != NULL)
                system_commands(fg_command, 0);
        }
        else
        {
            system_commands(command, 0);
        }
        

        // printf("ERROR: '%s' is not a valid command\n", cmd);
    }
    // printf(">>\n");
}

void Input(const char *input, char *prev_wd, char *home_for_shell, char *cwd, char *path_to_historyfile)
{
    char delim[] = ";\n";
    char input_copy[MAX_COMMAND_LENGTH];
    strncpy(input_copy, input, sizeof(input_copy) - 1);
    input_copy[sizeof(input_copy) - 1] = '\0';

    int cmd_count = 0;
    char commands[MAX_COMMANDS][MAX_COMMAND_LENGTH];

    char *cmd = strtok(input_copy, delim);
    while (cmd != NULL && cmd_count < MAX_COMMANDS)
    {
        strncpy(commands[cmd_count], cmd, sizeof(commands[cmd_count]) - 1);
        commands[cmd_count][sizeof(commands[cmd_count]) - 1] = '\0';

        cmd = strtok(NULL, delim);
        cmd_count++;
    }

    for (int i = 0; i < cmd_count; i++)
    {
        // printf("%s\n",commands[i]);
        execute_commands(prev_wd, home_for_shell, cwd, path_to_historyfile, commands[i]);
    }
}

void input_requirements(char *input, char *prev_wd, char *home_for_shell, char *cwd, char *path_to_historyfile)
{

    for (int i = 0; i < bg_count; i++)
    {
        int status;
        pid_t result = waitpid(bg_processes[i].pid, &status, WNOHANG);
        if (result != 0)
        {
            if (WIFEXITED(status))
            {
                if (WEXITSTATUS(status) == 0)
                    printf("%s exited normally (PID %d)\n",bg_processes[i].command, bg_processes[i].pid);
                else
                {
                    printf("%s exited abnormally (PID %d)\n",bg_processes[i].command, bg_processes[i].pid);
                }
            }

            // Remove this process from the list
            bg_count--;
            bg_processes[i] = bg_processes[bg_count];
            i--;
        }
    }

    char *delimiter = " \n";
    char *cmd;
    // printf("%s\n",path_to_historyfile);
    char input1[1024];

    strcpy(input1, input);
    // printf("%s\n", input3);
    cmd = strtok(input1, "\n");
    add_command(input1, path_to_historyfile);
    int and = 0;
    for (int i = 0; i < strlen(input1); i++)
    {
        if (input1[i] == ';')
            and++;
    }
    if (and != 0)
    {
        Input(input1, prev_wd, home_for_shell, cwd, path_to_historyfile);
    }
    else
        execute_commands(prev_wd, home_for_shell, cwd, path_to_historyfile, input1);
}