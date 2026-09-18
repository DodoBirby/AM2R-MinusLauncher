/*
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
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
        execl(scriptPath, scriptName, (char*)NULL);
        exit(0);
    }
    else
    { // parent
        if (pid < 0) perror("Failed to launch profile");

        printf("script: %s name: %s\n", scriptPath, scriptName);
        sleep(1); // give the user the illusion of the program doing something
    }
    puts("end execution");
}
*/
