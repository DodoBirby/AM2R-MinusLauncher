#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include "launcher.h"

char* profileDir = "profiles"; // TODO: config option
char* cuProfile = "CommunityUpdates";

// count number of profiles. all strings and the array must be manually freed
char** GetProfiles(int* out_count)
{
    *out_count = 0;
    int check = CheckFile(profileDir);
    if (check == ENOENT)
    {
        mkdir(profileDir, 0777);
        return NULL;
    }
    else if (check != 0)
    {
        perror("Failed to stat the profiles directory");
        exit(1);
    }

    int size = 8;
    char** dirs = calloc(size, sizeof(char*));
    DIR* dir = opendir(profileDir);
    struct dirent* ent;
    while ((ent = readdir(dir)) != NULL)
    {
        if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))
        {
            dirs[*out_count] = malloc(strlen(ent->d_name) + 1);
            strcpy(dirs[*out_count], ent->d_name);
            (*out_count)++;
            if (*out_count >= size)
            {
                size *= 2;
                dirs = realloc(dirs, size * sizeof(char*));
            }
        }
    }
    return dirs;
}

// check if CU is installed
bool IsCUInstalled()
{
    char* dir = PathCat(profileDir, cuProfile);
    int check = CheckFile(dir);
    free(dir);
    return check == 0;
}

int GetProfileCount()
{
    int count;
    char** arr = GetProfiles(&count);
    for (int i = 0; i < count; i++)
        free(arr[i]);

    free(arr);
    return count;
}
