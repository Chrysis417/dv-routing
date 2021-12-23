#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <iostream>

int read_user_cmd( char * cmd_buf, int buf_len)
{
    fd_set rfds;
    struct timeval tv;
    int retval;
    int cmd_len;

    /* Watch stdin (fd 0) to see when it has input. */

    FD_ZERO(&rfds);
    FD_SET(0, &rfds);

    /* Wait up to 0 seconds. */
    tv.tv_sec = 0;
    tv.tv_usec = 200;

    retval = select(1, &rfds, NULL, NULL, &tv);
    /* Don't rely on the value of tv now! */

    if (retval == -1){
        return 0;
    }else if (retval){
        if (FD_ISSET(0, &rfds) ){
            cmd_len = read(0, cmd_buf, buf_len);
            cmd_buf[cmd_len-1] ='\0';
            // std::cout << "cmd user input: " <<cmd_buf << std::endl;
            return 1;
        }
    }
    else return 0;
    
    return 0;

}