#include "prompt.h"

void printFileContents(const char *filePath)
{
    FILE *file = fopen(filePath, "r");
    if (file == NULL)
    {
        perror("Failed to open file");
        return;
    }

    char buffer[4096];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        fwrite(buffer, 1, bytesRead, stdout);
    }

    fclose(file);
}

// int countFilesInDirectory(const char *directoryPath)
// {
//     DIR *dir = opendir(directoryPath);
//     if (dir == NULL)
//     {
//         perror("Failed to open directory");
//         return -1; // Return -1 to indicate an error
//     }

//     int fileCount = 0;
//     struct dirent *entry;
//     while ((entry = readdir(dir)) != NULL)
//     {
//         char entryPath[4096];
//         snprintf(entryPath, sizeof(entryPath), "%s/%s", directoryPath, entry->d_name);

//         struct stat st;
//         if (stat(entryPath, &st) == 0 && S_ISREG(st.st_mode))
//         {
//             fileCount++; // Regular file found
//         }
//     }

//     closedir(dir);
//     return fileCount;
// }

void searchInDirectory(const char *targetName, const char *targetDir, int isFile, int isDirectory, int isExecute, int isPrint)
{
    // printf("here!!\n");
    // printf("isFile:%d\n",isFile);
    // printf("isDirectory:%d\n",isDirectory);
    // printf("isExecute:%d\n",isExecute);
    // printf("isPrint:%d\n",isPrint);

    DIR *dir = opendir(targetDir);
    if (dir == NULL)
    {
        perror("Failed to open directory");
        printf("Directory path: %s\n", targetDir); // Print the directory path
        exit(1);
    }
    // printf("here!!\n");
    // file_count = 0;
    // struct dirent *entry1;
    // while ((entry1 = readdir(dir)) != NULL)
    // {
    //     if (entry1->d_name[0] == '.')
    //         continue; // Skip hidden files and directories

    //     char entryPath1[4096];
    //     snprintf(entryPath1, sizeof(entryPath1), "%s/%s", targetDir, entry1->d_name);
    //     // printf("here!!\n");
    //     // printf("%s\n", entry->d_name);
    //     struct stat st1;
    //     if (stat(entryPath1, &st1) == 0)
    //     {
    //         if (S_ISDIR(st1.st_mode))
    //         {
    //             file_count++;
    //         }
    //     }
    // }
    char dir_name[1024];
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name[0] == '.')
            continue; // Skip hidden files and directories

        char entryPath[4096];
        snprintf(entryPath, sizeof(entryPath), "%s/%s", targetDir, entry->d_name);
        // printf("here!!\n");
        // printf("%s\n", entry->d_name);
        struct stat st;
        if (stat(entryPath, &st) == 0)
        {
            // printf("%s\n", entry->d_name);

            if (isExecute && isFile)
            {
                // printf("here!!\n");

                if (S_ISDIR(st.st_mode))
                {
                    // printf("here!!\n");
                    seek_count++;
                    searchInDirectory(targetName, entryPath, isFile, isDirectory, isExecute, isPrint);
                }
                else if (seek_count == 1 && strncmp(targetName, entry->d_name, strlen(targetName)) == 0)
                {
                    printFileContents(entryPath);
                }
                else
                {
                    return;
                }
            }
            if(isExecute && isDirectory)
            {
                if (S_ISDIR(st.st_mode) && strncmp(targetName, entry->d_name, strlen(targetName)) == 0)
                {
                    // printf("here!!\n");
                    dir_count++;
                    
                    strcpy(dir_name,entryPath);
                    // searchInDirectory(targetName, entryPath, isFile, isDirectory, isExecute, isPrint);
                }
            }
            if (S_ISDIR(st.st_mode) && isFile && !isDirectory && !isExecute)
            {
                searchInDirectory(targetName, entryPath, isFile, isDirectory, isExecute, isPrint);
            }
            if (S_ISREG(st.st_mode) && isFile & !isExecute)
            {
                // printf("%s %s\n", entry->d_name,targetName);

                if (strncmp(targetName, entry->d_name, strlen(targetName)) == 0)
                {
                    // printf("HERE!!\n");
                    if (isExecute && access(entryPath, X_OK) != 0)
                    {
                        printf("Missing permissions for task!\n");
                        closedir(dir);
                        return;
                    }
                    if (isPrint)
                    {
                        FILE *file = fopen(entryPath, "r");
                        if (file != NULL)
                        {
                            char line[1024];
                            while (fgets(line, sizeof(line), file))
                            {
                                printf("%s", line);
                            }
                            fclose(file);
                        }
                    }
                    else
                    {
                        printf("%s\n", entryPath);
                    }
                }
            }
            else if (S_ISDIR(st.st_mode) && isDirectory && !isExecute)
            {
                if (strncmp(targetName, entry->d_name, strlen(targetName)) == 0)
                {
                    if (isExecute && access(entryPath, X_OK) != 0)
                    {
                        printf("Missing permissions for task!\n");
                        closedir(dir);
                        return;
                    }
                    // printf("Hi\n");
                    // chdir(entryPath);
                    printf("%s\n", entryPath);
                }
                searchInDirectory(targetName, entryPath, isFile, isDirectory, isExecute, isPrint);
            }
        }
    }
    if(dir_count==1)
    {
        chdir(dir_name);
    }

    closedir(dir);
}

void seek_cmd(char *command)
{
    char *args[10]; // Adjust the size based on expected argument count
    int count = 0;
    // printf("%s\n",command);
    char *token = strtok(command, " ");
    while (token != NULL)
    {
        args[count++] = token;
        token = strtok(NULL, " ");
    }

    if (count < 2 || count > 5)
    {
        printf("Invalid usage!\n");
        printf("Usage: %s <search> <target_directory> [-d] [-f] [-e]\n", args[0]);
        return;
    }

    int isFile = 1, isDirectory = 1, isExecute = 0, isPrint = 0;
    int i = 1;
    for (i = 1; i < count;)
    {
        if (strcmp(args[i], "-d") == 0)
        {
            isFile = 0;
            i++;
        }
        else if (strcmp(args[i], "-f") == 0)
        {
            isDirectory = 0;
            i++;
        }
        else if (strcmp(args[i], "-e") == 0)
        {
            isExecute = 1;
            i++;
        }
        else if (isDirectory == 0 && isFile == 0)
        {
            printf("Invalid flags!\n");
            return;
        }
        else if (strcmp(args[i], "-df") == 0 || strcmp(args[i], "-fd") == 0)
        {
            printf("Invalid flags!\n");
            return;
        }
        else
            break;
    }
    const char *targetName = args[i];
    i++;
    // if(i==1)
    // {
    //     i++;
    //     printf("Hlo\n");
    //     targetName = args[i];
    //     printf("Hlo\n");
    // }
    // else
    // {
    //     targetName = args[i];
    //     i++;
    // }
    char targetDir[1024];
    if (args[i] != NULL)
    {
        // printf("here!!\n");

        strcpy(targetDir, args[i]);
    }
    // targetDir = args[i];
    // printf("Hlo\n");
    // printf("%s\n",args[i]);
    else
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        // printf("%s\n", cwd);
        // targetDir = cwd;
        strcpy(targetDir, cwd);
        // printf("%s\n", targetDir);
    }
    // printf("%s\n",targetDir);

    searchInDirectory(targetName, targetDir, isFile, isDirectory, isExecute, isPrint);
}
