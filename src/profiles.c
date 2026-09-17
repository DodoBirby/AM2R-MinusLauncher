#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include "launcher.h"

char* profileDir = "profiles";
char* cuProfile = "CommunityUpdates";

// count number of profiles
int ProfileCount()
{
    int check = CheckFile(profileDir);
    if (check == ENOENT)
    {
        mkdir(profileDir, 0777);
        return 0;
    }
    else if (check != 0)
    {
        perror("Failed to stat the profiles directory");
        exit(1);
    }

    DIR* dir = opendir(profileDir);
    struct dirent* ent;
    int count = 0;
    while ((ent = readdir(dir)) != NULL)
    {
        if (ent->d_type == DT_DIR && strcmp(ent->d_name, ".") && strcmp(ent->d_name, ".."))
            count++;
    }
    return count;
}

// check if CU is installed
bool IsCUInstalled()
{
    char* dir = PathCat(profileDir, cuProfile);
    int check = CheckFile(dir);
    free(dir);
    return check == 0;
}
