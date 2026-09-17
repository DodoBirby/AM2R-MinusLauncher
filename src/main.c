#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "launcher.h"

int main()
{
    bool git = system("git --version &> /dev/null") == 0;
    bool xdelta = system("xdelta3 -V &> /dev/null") == 0;
    bool unzip = system("unzip &> /dev/null") == 0;
    if (!(git && xdelta && unzip))
    {
        if (!git) puts("FATAL: You must install git and have it on your PATH to use this program");
        if (!xdelta) puts("FATAL: You must install xdelta3 and have it on your PATH to use this program");
        if (!unzip) puts("FATAL: You must install unzip and have it on your PATH to use this program");
        exit(1);
    }

    if (!CheckPatchData())
        exit(1);

    if (!CheckForAM2R11()) // TODO: ask user to select 1.1 zip
    {
        puts("1.1 not found! Please unzip it next to the launcher in a directory named \"AM2R_11\"");
        exit(1);
    }

    if (!InstallMod("autopatcher/data"))
    {
        exit(1);
    }

    return 0;
}
