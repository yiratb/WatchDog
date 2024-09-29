/*//////////////////////////////////
File Name: wd_client.h
File Type: source
Made By: Yirat Romano
Review By: Itamar
Last Modefy Date: 20/04/24
////////////////////////////////////*/

#ifndef __ILRD_WD_1556__
#define __ILRD_WD_1556__

typedef enum wd_status
{
    WDSUCCESS = 0,
    WDFAILURE,
    FINISH,
    NOT_FINISH
} wd_status_t;

typedef enum identity
{
    SON = 0,
    FATHER
}identity_t;

/*
Description:
    -Defends a critical section: if this section crashes, the program is revived
Params:
    -cmd: command line to reinitiate the process {"./a.out", "arguments"...}
Return:
    -status:
        -SUCCESS: section is protected
        -FAILURE: section isn't protected
Notes:
    -this utility uses SIGUSR1 SIGUSR2 signals
*/
wd_status_t WDStart(const char **cmd);

/*
Description:
    -Ends the critical section
Notes:
    -Function does not guarentee to free used resources immediately at function exit
*/
void WDStop(void);

#endif /* __ILRD_WD_1556__ */