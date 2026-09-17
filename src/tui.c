#include <stdio.h>
#include <stdlib.h>
#include "launcher.h"

typedef struct
{
    char* opText;
    void (*selected)();
    bool closeAfter;

} MenuOption;

static void Op_Download()
{
    CloneAutopatcher();
}

static void Op_InstallCU()
{
    char* path = PathCat(patchDataPath, "data");
    InstallMod(path, cuProfile);
    free(path);
}

static void Op_Play() // TODO
{
}

bool menuInitialized = false;
MenuOption* op_download;
MenuOption* op_installCU;
MenuOption* op_play;

#define ITEMS 4
MenuOption* options[ITEMS];

static void InitializeMenu()
{

    op_download = malloc(sizeof(MenuOption));
    op_download->opText = "Download Patch Data";
    op_download->selected = Op_Download;
    op_download->closeAfter = false;

    op_installCU = malloc(sizeof(MenuOption));
    op_installCU->opText = "Install Community Updates";
    op_installCU->selected = Op_InstallCU;
    op_installCU->closeAfter = false;

    op_play = malloc(sizeof(MenuOption));
    op_play->opText = "Launch a profile";
    op_play->selected = Op_Play;
    op_play->closeAfter = true;
}

bool MainMenu()
{
    if (!menuInitialized) InitializeMenu();

    for (int i = 0; i < ITEMS; i++)
        options[i] = NULL;

    int pos = 0;
    if (!CheckPatchData(false)) // show download option
    {
        options[pos] = op_download;
        pos++;
    }
    if (!IsCUInstalled() && CheckPatchData(false)) // show install CU option
    {
        options[pos] = op_installCU;
        pos++;
    }
    if (ProfileCount() > 0) // show play option
    {
        options[pos] = op_play;
        pos++;
    }

    if (pos == 0)
    {
        puts("It seems you have no options. You've met with a terrible fate, haven't you?");
        exit(1);
    }

    puts("");
    for (int i = 0; i < ITEMS; i++)
    {
        if (options[i] == NULL) continue;
        printf("[%d] ", i + 1);
        puts(options[i]->opText);
    }

    char in[32];
    fgets(in, 32, stdin);
    fflush(stdin);
    int choice = atoi(in) - 1;

    if (choice < 0 || choice > ITEMS || options[choice] == NULL)
    {
        printf("Invalid choice: %s\n", in);
        return true;
    }
    options[choice]->selected();
    return !(options[choice]->closeAfter);
}

void DestroyMenu()
{
    free(op_download);
    free(op_installCU);
    free(op_play);
}
