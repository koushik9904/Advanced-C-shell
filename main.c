#include "prompt.h"
#include "input.c"
#include "prompt.c"
#include "warp.c"
#include "peek.c"
#include "pastevents.c"
#include "proclore.c"
#include "system_commands.c"
#include "seek.c"
#include "redirection.c"
#include "pipe.c"
#include "activities.c"
#include "signals.c"
#include "bg.c"
#include "fg.c"
#include "iMan.c"
#include "neonate.c"


int main()
{
    // Keep accepting commands
    struct utsname *sys_info = (struct utsname *)malloc(sizeof(struct utsname));
    uname(sys_info);
    char *sys_name = strdup(sys_info->nodename);
    struct passwd *user_details = getpwuid(getuid());
    char *username = user_details->pw_name;
    char prev_wd[1024];
    char cwd[1024];
    char home_for_shell[1024];
    getcwd(cwd, sizeof(cwd));
    strcpy(home_for_shell, cwd);
    char path_to_historyfile[1024];
    strcpy(prev_wd, cwd);
    strcpy(path_to_historyfile,cwd);
    strcat(path_to_historyfile,"/history.txt");
    load_history(path_to_historyfile);
    char *prev_input;

    signal(SIGINT, Ctrl_C);
    signal(SIGTSTP, Ctrl_Z);

    while (1)
    {
        // Print appropriate prompt with username, systemname and directory before accepting input
        getcwd(cwd, sizeof(cwd));

        prompt(home_for_shell, cwd, prev_wd, username, sys_name,flag,prev_input);
        flag=0;
        char input[4096];
        if(fgets(input, 4096, stdin)==NULL)
            Ctrl_D();
        char *ip_cpy = strdup(input);

        input_requirements(input,prev_wd,home_for_shell,cwd,path_to_historyfile);
        prev_input = strdup(ip_cpy);
    }
    // printf("KK\n");
    return 0;
}
