#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include "launcher.h"

char* repo = "https://github.com/AM2R-Community-Developers/AM2R-Autopatcher-Linux.git";
char* patchDataPath = "autopatcher";
char* pathTo11 = "AM2R_11";

static bool CloneAutopatcher()
{
    puts("DEBUG: pretend we cloned it :3");
    return true;

    mkdir(patchDataPath, 0777);
    int size = strlen(patchDataPath) + strlen(repo) + 32;
    char cmd[size];
    sprintf(cmd, "git clone --depth 1 %s %s", repo, patchDataPath);
    int code = system(cmd);
    return code == 0;
}

// returns true on existing or just cloned, false on error
bool CheckPatchData()
{
    // check if the path data contains the 1.5.5 "mod" folder
    char* statPath = PathCat(patchDataPath, "data");
    int result = CheckFile(statPath);
    free(statPath);
    if (result)
    {
        if (result != ENOENT) // if the error is something other than not existing
        {
            printf("FATAL: Statting directory %s/data failed with unhandled error %d\n", patchDataPath, errno);
            return false;
        }
        puts("Cloning patch data repository...");
        if (!CloneAutopatcher())
        {
            puts("FATAL: git clone failed");
            printf("(hint: try deleting the %s directory)\n", patchDataPath);
            return false;
        }
    }
    else
    {
        puts("patch data already exists, yay");
    }

    return true;
}

// not very sophisticated, i know, and no integirty checks either
bool CheckForAM2R11()
{
    char* statPath = PathCat(pathTo11, "data.win");
    if (CheckFile(statPath))
    {
        free(statPath);
        return false;
    }
    free(statPath);
    return true;
}
