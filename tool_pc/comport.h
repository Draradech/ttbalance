#ifndef _COMPORT_H
#define _COMPORT_H

#ifdef linux
#else
#include <windows.h>
#endif

#include <string.h>

int openComPort(const char* port);

void closeComPort (void);

void sendByte(unsigned char byte);

int receiveByte(unsigned char* byte); 

#endif /* _COMPORT_H */
