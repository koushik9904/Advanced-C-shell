#ifndef __PROMPT_H
#define __PROMPT_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sysmacros.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/wait.h>
#include <signal.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <termios.h>



#define STATUS_PATH "/proc/%d/status"
#define EXECUTABLE_PATH "/proc/%d/exe"


long int flag = 0;

pid_t fg_pid = 0;

int foreground = 0;

struct BackgroundProcess
{
    pid_t pid;
    int running;
    // time_t start_time;
    char command[1024];
};

struct BackgroundProcess bg_processes[100]; 
int bg_count = 0;


int seek_count=0;
int file_count;
int dir_count=0;


void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}


void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}
// struct BackgroundProcess bg_processes_list[100]; 
// int bg_count_list = 0;

void input_requirements(char *input,char *prev_wd,char *home_for_shell,char *cwd,char *path_to_historyfile);
void execute_commands(char *prev_wd, char *home_for_shell, char *cwd, char *path_to_historyfile, char *input);
void Input(const char *input, char *prev_wd, char *home_for_shell, char *cwd, char *path_to_historyfile);


void load_history(char *path_to_history);
void save_history(char *path_to_historyfile);
void add_command(const char *cmd,char *path_to_historyfile);
void print_history();
void clear_history(char *path_to_historyfile);
char *pastevent_execute(int num);
int isSubstring(const char *str, char *sub);


void printdir_details(char *file_path,char *file_name);
void peek_cmd(int flag_l, int flag_a, char *path);


void redirect(char *command,char *input, char *prev_wd, char *home_for_shell, char *cwd, char *path_to_historyfile);


void searchInDirectory(const char *targetName, const char *targetDir, int isFile, int isDirectory, int isExecute, int isPrint);
void seek_cmd(char *command);


void system_commands(char *command,int background);


void last_slash(char *input);
char *find_path(char *cmd, char *prev_wd,char *home_for_shell);
void warp_cmd(char *cmd, char *input, char *prev_wd,char *home_for_shell);
char *warp_for_peek(char *cmd, char *input, char *prev_wd,char *home_for_shell);
void Replace(char *path, char *home, char replacement);


void pipes(char *command,char *prev_wd, char *home_for_shell, char *cwd, char *path_to_historyfile);


void get_process_info(int pid, char *home_for_shell, char *cwd);


void activity();


void ping(char *command);
void Ctrl_C(int signal_number);
void Ctrl_D();
void Ctrl_Z(int signal_number);


void bg(int pid);


void fg(int pid);


void iMan_cmd(const char *command_name);


void Neonate(int time);
int getMostRecentPID();



#endif