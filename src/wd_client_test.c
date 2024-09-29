/*//////////////////////////////////
File Name: wd_client_test.c
File Type: source
Made By: Yirat Romano
Review By: Itamar
Last Modefy Date: 20/04/24
////////////////////////////////////*/

#include <stdio.h> /* printf() */
#include <stdlib.h> 
#include <unistd.h> 
#include <time.h>

#include "../inc/wd.h"
 

int main(int argc, const char *argv[])
{
    const char *cmd[3] = {0};
    time_t start_time = time(NULL);

    (void)argc;
    cmd[0] = "./wd_client.out";
    cmd[1] = (char *)argv;
    cmd[2] = 0; 

    WDStart(cmd);
    printf("Critical code section\n");

    while(start_time + 20 > time(NULL))
    {
        sleep(1);
    }

    WDStop();

    return 0;
}