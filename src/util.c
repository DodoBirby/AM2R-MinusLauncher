#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>

// returns 0 on found, otherwise errno
int CheckFile(char* path)
{
    struct stat dirCheck;
    if (stat(path, &dirCheck) == -1)
    {
        return errno;
    }
    return 0;
}

// creates a dynamically allocated string in the form of "s1/s2". you must free it manually!
char* PathCat(const char* s1, const char* s2)
{
    char* str = (char*)malloc(strlen(s1) + strlen(s2) + 2);
    strcpy(str, s1);
    strcat(str, "/");
    strcat(str, s2);
    return str;
}

// returns 0 on success, otherwise errno
int LowercaseFile(const char* name, const char* path)
{
    int len = strlen(name);
    char lowered[len + 1];
    strcpy(lowered, name);
    for (int i = 0; i < len; i++)
        lowered[i] = tolower(lowered[i]);

    char* oldName = PathCat(path, name);
    char* newName = PathCat(path, lowered);
    //printf("%s %s: ", oldName, newName);
    if (strcmp(oldName, newName))
    {
        //puts("not equal");
        if (rename(oldName, newName))
        {
            free(oldName);
            free(newName);
            return errno;
        }
    }
    //else puts("equal");
    free(oldName);
    free(newName);
    return 0;
}
