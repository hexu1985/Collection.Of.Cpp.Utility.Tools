#include <stdio.h>
#include "config.h"

int main()
{
    printf("version: %s\n", PROJECT_VERSION);
    printf("commit:  %s\n", GIT_COMMITID);
    printf("branch:  %s\n", GIT_BRANCH);
    printf("build time: %s\n", BUILD_TIME);
    printf("build no: %s\n", BUILD_NO);
    return 0;
}
