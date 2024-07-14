#include "prompt.h"


void last_slash(char *input)
{
    char *lastSlash = strrchr(input, '/');
    if (lastSlash)
    {
        *lastSlash = '\0';
    }
}

char *find_path(char *cmd, char *prev_wd, char *home_for_shell)
{
    // char *cwd = (char*)malloc(sizeof(char)*1024);
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    char *path = (char *)malloc(sizeof(char) * 1024);
    char *dots = "..";
    char *slash = "/";
    char *dot = ".";
    // char *null = "\0";
    if (cmd[0] == '~')
    {
        // printf("~\n");
        // char *home_dir = getenv("HOME");
        strcpy(path, home_for_shell);
        // path = home_dir;
        if (cmd[1] != '\0')
        {
            char *new_dir = cmd + 1;
            strcat(path, cmd + 1);
        }
    }
    else if (strcmp(cmd, dots) == 0)
    {
        // printf("..\n");
        // path = removeLastSlash(cwd);
        last_slash(cwd);
        strcpy(path, cwd);
    }
    else if (strcmp(cmd, "-") == 0)
    {
        // printf("-\n");
        path = prev_wd;
    }
    else if (strcmp(cmd, ".") == 0)
    {
        strcpy(path, cwd);
        return path;
    }
    else if(cmd[0]=='/')
    {
        strcpy(path,cmd);
    }
    else
    {
        // printf("Hi\n");
        strcat(cwd, slash);
        // printf("%s",cwd);
        strcat(cwd, cmd);
        // printf("%s",cwd);
        strcpy(path, cwd);
    }
    // strcat(path, null);
    return path;
}


void warp_cmd(char *cmd, char *input, char *prev_wd, char *home_for_shell)
{
    char *path = find_path(cmd, prev_wd, home_for_shell);
    char tilda = '~';
    int kk = chdir(path);
    if (kk == -1)
    {
        perror("chdir");
       
    }
    else
    {   
        // Replace(path, home_for_shell,tilda);
        printf("%s\n", path);}
}

char *warp_for_peek(char *cmd, char *input, char *prev_wd, char *home_for_shell)
{
    char *path = find_path(cmd, prev_wd, home_for_shell);
    return path;
}
