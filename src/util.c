#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <linux/limits.h>

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
    if (strcmp(oldName, newName))
    {
        if (rename(oldName, newName))
        {
            free(oldName);
            free(newName);
            return errno;
        }
    }
    free(oldName);
    free(newName);
    return 0;
}

void GetInput(char* buf, int max)
{
    //char c;
    //while ((c = getchar()) != '\n' && c != EOF) {}    UUUUUUUUGGHHHHHHHHH JUST WORKKKKKKK
    fgets(buf, max, stdin);
    buf[strlen(buf) - 1] = '\0';
}

bool GetAndUnzipFile(char* outputPath)
{
    char in[PATH_MAX];

    while (true)
    {
        fputs("Path: ", stdout);
        GetInput(in, PATH_MAX);

        int result = CheckFile(in);
        if (result != 0)
        {
            perror("Failed to read provided file");
            printf("DEBUG: provided path was %s\n", in);
            continue;
        }

        char checkZip[strlen(in) + 25];
        sprintf(checkZip, "unzip -Z %s &> /dev/null", in);
        if (system(checkZip))
        {
            puts("unzip could not recognize this as a valid .zip archive. Please try another path.");
            continue;
        }
        break;
    }

    char unzipCall[strlen(in) + strlen(outputPath) + 23];
    sprintf(unzipCall, "unzip -d \"%s\" \"%s\"", outputPath, in);
    if (system(unzipCall))
        return false;

    return true;
}
