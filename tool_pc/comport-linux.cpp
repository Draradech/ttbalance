#include "comport.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>

#define BAUDRATE B115200

#define TRUE 1
#define FALSE 0

int fd;

int openComPort (const char* port)
{
    struct termios tio;

    fd = open(port, O_RDWR | O_NOCTTY ); 
    if (fd < 0)
    {
        perror(port);
        exit(-1);
    }

    bzero(&tio, sizeof(tio));
    tio.c_cflag = BAUDRATE | CS8 | CLOCAL | CREAD;
    tio.c_iflag = IGNPAR;
    tio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    tio.c_lflag = 0;

    tio.c_cc[VTIME]    = 0;   /* timeout immediately */
    tio.c_cc[VMIN]     = 0;   /* read single characters */

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &tio);
    
    return 1;
}  

void closeComPort(void)
{
}

void sendByte(unsigned char byte)
{
   write(fd, &byte, 1);
}

int receiveByte(unsigned char* byte)
{
   return read(fd, byte, 1);
}

