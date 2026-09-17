#ifndef __AM2RLAUNCHER_MINUS_H
#define __AM2RLAUNCHER_MINUS_H

// util.c
int CheckFile(char* path);
char* PathCat(const char* s1, const char* s2);
int LowercaseFile(const char* name, const char* path);

// setup.c
extern char* repo;
extern char* patchDataPath;

bool CheckPatchData();
bool CheckForAM2R11();

// patcher.c
bool InstallMod(char* modPath);

#endif
