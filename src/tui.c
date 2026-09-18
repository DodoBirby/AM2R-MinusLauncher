#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

static void Op_InstallMod() // TODO
{
    puts("TODO, sorry");
}

static void Op_Play()
{
    int count;
    char** profiles = GetProfiles(&count);
    int choice;
    while (true)
    {
        puts("\nChoose a profile to play:\n");
        for (int i = 0; i < count; i++)
        {
            printf("[%d] %s\n", i + 1, profiles[i]);
        }

        fputs("\nProfile number: ", stdout);

        char in[32];
        GetInput(in, 32);
        choice = atoi(in) - 1;

        if (choice >= 0 && choice < count) break;

        printf("Invalid choice: %s\n", in);
    }

    printf("Opening profile %s...\n", profiles[choice]);
    fflush(stdout);

    // pre-calc these because the child is only allowed to touch the PID and call exec
    char* profilePath = PathCat(profileDir, profiles[choice]);
    char* scriptName = "run-with-libs.sh";
    char* scriptPath = PathCat(profilePath, scriptName);
    pid_t pid;

    char popDir[PATH_MAX];
    getcwd(popDir, PATH_MAX);
    chdir(profilePath);

    if ((pid = vfork()) == 0)
    { // child
        execl(scriptName, scriptName, (char*)NULL);
        exit(0);
    }
    else
    { // parent
        if (pid < 0) perror("Failed to launch profile");
        sleep(1); // give the user the illusion of the program doing something
    }

    chdir(popDir);

    free(scriptPath);
    free(profilePath);
    for (int i = 0; i < count; i++)
        free(profiles[i]);
    free(profiles);
}

bool menuInitialized = false;
MenuOption* op_download;
MenuOption* op_installCU;
MenuOption* op_installMod;
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

    op_installMod = malloc(sizeof(MenuOption));
    op_installMod->opText = "Install a mod";
    op_installMod->selected = Op_InstallMod;
    op_installMod->closeAfter = false;

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
    if (GetProfileCount() > 0) // show play option
    {
        options[pos] = op_play;
        pos++;
    }
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
    if (true) // show install mod option
    {
        options[pos] = op_installMod;
        pos++;
    }

    if (pos == 0)
    {
        puts("It seems you have no options. You've met with a terrible fate, haven't you?");
        exit(1);
    }

    puts("\nChoose an option:\n");
    for (int i = 0; i < ITEMS; i++)
    {
        if (options[i] == NULL) continue;
        printf("[%d] ", i + 1);
        puts(options[i]->opText);
    }
    fputs("\nOption number: ", stdout);

    char in[32];
    GetInput(in, 32);
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
