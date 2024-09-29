/*//////////////////////////////////
File Name: wd.c
File Type: source
Made By: Yirat Romano
Review By: Itamar
Last Modefy Date: 20/04/24
////////////////////////////////////*/

/*gd -pthread ../../src/wd.c ../../src/wd_client.c ../../src/scheduler.c ../../src/task.c ../../src/uid.c ../../src/srtlist.c ../../src/dlist.c ../../src/pqueue.c -I ../../inc/ -o wd*/

#include "wd.h"
#include <stdio.h>


int main(int argc, char const *argv[])
{
    const char *args[3] = {0};

    (void)argc;

    args[0] = "./wd";
    args[1] = (char *) (argv);
    args[2] = 0;

    WDStart(args);
    return (0);
}