#include <stdio.h>
#include <time.h>


int debug_print(char *text) {
#ifdef DEBUG
    time_t rawtime;
    struct tm * timeinfo;
    time (&rawtime);
    timeinfo = localtime(&rawtime);

    printf("\033[0m(%d:%d) - [\033[94mHumble\033[0m/\033[94mrelease\033[0m]: %s\n", timeinfo->tm_hour, timeinfo->tm_min, text);
#endif
}
