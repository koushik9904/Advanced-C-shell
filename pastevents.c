#include "prompt.h"


// #define path_to_historyfile path_to_historyfile

typedef struct
{
    char command[1024];
} Command;

Command history[15];
int count = 0;

int isSubstring(const char *str, char *sub)
{
    int str_len = strlen(str);
    int sub_len = strlen(sub);

    for (int i = 0; i <= str_len - sub_len; i++)
    {
        int j;
        for (j = 0; j < sub_len; j++)
        {
            if (str[i + j] != sub[j])
                break;
        }
        if (j == sub_len)
            return 1;
    }

    return 0;
}

void load_history(char *path_to_historyfile)
{
    FILE *file = fopen(path_to_historyfile, "r");
    if (file)
    {
        char line[1024];
        while (fgets(line, sizeof(line), file))
        {
            if (count < 15)
            {
                line[strcspn(line, "\n")] = '\0';
                strcpy(history[count].command, line);
                count++;
            }
        }
        fclose(file);
    }
}

void save_history(char *path_to_historyfile)
{
    FILE *file = fopen(path_to_historyfile, "w");
    if (file)
    {
        for (int i = 0; i < count; i++)
        {
            fprintf(file, "%s\n", history[i].command);
        }
        fclose(file);
    }
}

void clear_history(char *path_to_historyfile)
{
    FILE *file = fopen(path_to_historyfile, "w");
    if (file == NULL)
    {
        perror("Failed to open the file");
        return;
    }
    fclose(file);
    count = 0;
}

void add_command(const char *cmd, char *path_to_historyfile)
{
    char *pastevent = "pastevents";
    if (isSubstring(cmd, pastevent) == 0)
    {
        if (count > 0 && strcmp(cmd, history[count - 1].command) == 0)
        {
            return;
        }

        if (count == 15)
        {
            for (int i = 0; i < 15 - 1; i++)
            {
                strcpy(history[i].command, history[i + 1].command);
            }
            count--;
        }

        strcpy(history[count].command, cmd);
        count++;

        save_history(path_to_historyfile);
    }
    else
        return;
}

void print_history()
{
    for (int i = 0; i <= count - 1; i++)
    {
        printf("%s\n", history[i].command);
    }
}

char *pastevent_execute(int num)
{
    char *event = (char *)malloc(sizeof(char) * 1024);
    strcpy(event, history[count - num].command);
    return event;
}
