#include "prompt.h"


int compare_filenames(const void *a, const void *b)
{
    const char *str_a = *(const char **)a;
    const char *str_b = *(const char **)b;

    while (*str_a && *str_b)
    {
        char char_a = tolower(*str_a);
        char char_b = tolower(*str_b);

        if (char_a != char_b)
        {
            return char_a - char_b;
        }

        str_a++;
        str_b++;
    }

    return *str_a - *str_b;
}

void printdir_details(char *file_path, char *file_name)
{
    struct stat data_file;
    // printf("l flag\n");

    if (stat(file_path, &data_file) == 0)
    {
        if (S_ISDIR(data_file.st_mode))
            printf("d");
        else
            printf("-");

        if (data_file.st_mode & S_IRUSR)
            printf("r");
        else
            printf("-");

        if (data_file.st_mode & S_IWUSR)
            printf("w");
        else
            printf("-");

        if (data_file.st_mode & S_IXUSR)
            printf("x");
        else
            printf("-");

        if (data_file.st_mode & S_IRGRP)
            printf("r");
        else
            printf("-");

        if (data_file.st_mode & S_IWGRP)
            printf("w");
        else
            printf("-");

        if (data_file.st_mode & S_IXGRP)
            printf("x");
        else
            printf("-");

        if (data_file.st_mode & S_IROTH)
            printf("r");
        else
            printf("-");

        if (data_file.st_mode & S_IWOTH)
            printf("w");
        else
            printf("-");

        if (data_file.st_mode & S_IXOTH)
            printf("x");
        else
            printf("-");

        printf(" ");

        printf("%ld ", data_file.st_nlink);

        struct passwd *pwd = getpwuid(data_file.st_uid);
        struct group *grp = getgrgid(data_file.st_gid);
        printf("%s ", (pwd) ? pwd->pw_name : "");
        printf("%s ", (grp) ? grp->gr_name : "");

        printf("%lld ", (long long)data_file.st_size);

        struct tm *latest_mod_time = localtime(&data_file.st_mtime);
        char time_str[20];
        strftime(time_str, sizeof(time_str), "%b %d %H:%M", latest_mod_time);
        printf("%s ", time_str);

        const char *color = (S_ISDIR(data_file.st_mode)) ? "\x1B[34;1m" : ((data_file.st_mode & S_IXUSR) || (data_file.st_mode & S_IXGRP) || (data_file.st_mode & S_IXOTH)) ? "\x1B[32;1m"
                                                                                                                                                                            : "\x1B[0m";
        printf("%s%s%s\n", color, file_name, "\x1B[0m");
    }
}

void peek_cmd(int flag_l, int flag_a, char *path)
{
    DIR *directory = opendir(path);
    if (directory == NULL)
    {
        perror("opendir");
        return;
    }

    struct dirent *details;
    char *color;
    char **file_names = NULL;
    int count = 0;

    while ((details = readdir(directory)) != NULL)
    {
        if (details->d_name[0] == '.' && flag_a == 0)
            continue;

        file_names = (char **)realloc(file_names, (count + 1) * sizeof(char *));
        if (file_names == NULL)
        {
            perror("realloc");
            return;
        }
        file_names[count] = strdup(details->d_name);
        count++;
    }

    qsort(file_names, count, sizeof(char *), compare_filenames);

    for (int i = 0; i < count; i++)
    {
        if (flag_l == 1)
        {
            char file_path[1024];
            snprintf(file_path, sizeof(file_path), "%s/%s", path, file_names[i]);
            printdir_details(file_path, file_names[i]);
        }
        else
        {
            char file_path[1024];
            snprintf(file_path, sizeof(file_path), "%s/%s", path, file_names[i]);
            struct stat data_file;
            stat(file_path, &data_file);
            const char *color = (S_ISDIR(data_file.st_mode)) ? "\x1B[34;1m" : ((data_file.st_mode & S_IXUSR) || (data_file.st_mode & S_IXGRP) || (data_file.st_mode & S_IXOTH)) ? "\x1B[32;1m"
                                                                                                                                                                                : "\x1B[0m";
            printf("%s%s%s\n", color, file_names[i], "\x1B[0m");
        }
    }

    for (int i = 0; i < count; i++)
    {
        free(file_names[i]);
    }
    free(file_names);

    closedir(directory);
}
