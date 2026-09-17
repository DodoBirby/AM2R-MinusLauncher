#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "launcher.h"

static bool LowercaseIteration(const char* path, char* dirs[], int* dirCount); // i don't want to re-organize things

const int MAXDIRS = 128; // cap on the number of recursive directories in a mod file

static bool PatchFile(char* src, char* patch, char* dest)
{
    char cmd[strlen(src) + strlen(patch) + strlen(dest) + 32];
    sprintf(cmd, "xdelta3 -dfs %s %s %s", src, patch, dest);

    return system(cmd) == 0;
}

static bool LowercaseRecursively(char* path)
{
    bool fail = false;
    char* dirs[MAXDIRS];
    for (int i = 0; i < MAXDIRS; i++) dirs[i] = NULL;
    dirs[0] = path;
    int dirCount = 0;

    int i = 0;
    while (dirs[i] != NULL)
    {
        if (!LowercaseIteration(dirs[i], dirs, &dirCount))
        {
            fail = true;
            break;
        }
        if (++i >= MAXDIRS) break;
    }
    if (fail) return false;

    // iterate all directories in reverse to lowercase them
    // this should work only because we added all the dirs breadth-first, and we iterate the array in reverse
    for (int i = MAXDIRS - 1; i >= 0; i--)
    {
        if (dirs[i] == NULL) continue;
        char* snip = strrchr(dirs[i], '/');
        if (snip != NULL) // if this doesn't pass, it's hopefully the mod root, so i ignore it with cope in my heart
        {
            char dirPath[strlen(dirs[i]) + 1];
            strncpy(dirPath, dirs[i], snip - dirs[i]);
            int result = LowercaseFile(dirs[i], dirPath);
            if (result)
            {
                printf("Install failed, could not rename %s to lowercase: error %d\n", dirs[i], errno);
                fail = true;
            }
        }
        free(dirs[i]); // thanks C standard library for defining the behavior when trying to free NULL
    }
    return !fail;
}

static bool LowercaseIteration(const char* path, char* dirs[], int* dirCount)
{
    DIR* dir = opendir(path);
    struct dirent* ent;
    bool fail = false;
    while ((ent = readdir(dir)) != NULL)
    {
        if (ent->d_type == DT_DIR)
        {
            // make sure it's not implied dirs . or ..
            if (strcmp(ent->d_name, ".") && strcmp(ent->d_name, "..")) // if both checks retrun non-zero, Proceed.
            {
                dirs[*dirCount] = PathCat(path, ent->d_name);
                printf("tracking dir %s at position %d\n", dirs[*dirCount], *dirCount);
                (*dirCount)++;
                if (*dirCount >= MAXDIRS)
                {
                    printf("PANIC: MAXDIRS (%d) directories tracked!!! Any more would clobber memory or segfault!\n", MAXDIRS);
                    return false;
                }
            }
            else continue;
        }
        else if (ent->d_type == DT_REG)
        {
            int result = LowercaseFile(ent->d_name, path);
            if (result)
            {
                printf("Install failed, could not rename %s to lowercase: error %d\n", ent->d_name, errno);
                return false;
            }
        }
        printf("%s %d\n", ent->d_name, ent->d_type);
    }
    return !fail;
}

bool InstallMod(char* modPath) // TODO: auto unzip provided mods
{
    char* profilePath = "profiles/modtest"; // TODO: parameterize
    // make sure this mod does not already exist
    if (!CheckFile(profilePath))
    {
        printf("Mod path %s exists, please delete it first if you wish to re-install it\n", profilePath);
        return false;
    }

    mkdir(profilePath, 0777);
    char* assetsPath = PathCat(profilePath, "assets");
    mkdir(assetsPath, 0777);
    if (CheckFile(modPath))
    {
        printf("Install failed, path %s could not be read\n", modPath);
        return false;
    }
    // copy 1.1 files to the mod folder - fall back on coreutils cp for now
    char cmd[strlen(assetsPath) + 24];
    sprintf(cmd, "cp -r AM2R_11/* %s", assetsPath);
    if (system(cmd))
    {
        puts("Install failed, could not copy AM2R 1.1 to the mod folder");
        return false;
    }
    sprintf(cmd, "mv %s/AM2R.exe %s", assetsPath, profilePath);
    if (system(cmd))
    {
        puts("Install failed, could not move AM2R.exe");
        return false;
    }

    // set up the buffers for patch arguments
    char* src;
    char* patch;
    char* dest;

    // patch the exe
    src = PathCat(profilePath, "AM2R.exe");
    patch = "autopatcher/data/AM2R.xdelta";
    dest = PathCat(profilePath, "runner");
    if (!PatchFile(src, patch, dest))
    {
        puts("Install failed, could not patch AM2R.exe");
        free(src);
        free(dest);
        return false;
    }
    free(src);
    free(dest);
    // patch the data.win
    src = PathCat(assetsPath, "data.win");
    patch = "autopatcher/data/game.xdelta";
    dest = PathCat(assetsPath, "game.unx");
    if (!PatchFile(src, patch, dest))
    {
        puts("Install failed, could not patch data.win");
        free(src);
        free(dest);
        return false;
    }
    free(src);
    free(dest);

    // copy everything from files_to_copy
    sprintf(cmd, "cp -rf autopatcher/data/files_to_copy/* %s", assetsPath);
    if(system(cmd))
    {
        puts("Install failed, could not copy mod files to the mod folder");
        return false;
    }

    // iterate the mod directory and lowercase all the files
    if (!LowercaseRecursively(assetsPath))
    {
        puts("Install failed, could not lowercase all files in the mod folder");
        return false;
    }


    return true;
}
