#include "comport.h"

static HANDLE _hCom;

int openComPort (const char* port)
{
  DCB dcb;       
  COMMTIMEOUTS timeouts = {0};

  _hCom = CreateFile(port,
                     GENERIC_READ | GENERIC_WRITE,
                     0,
                     0,
                     OPEN_EXISTING,
                     0,
                     0);

  if(_hCom == INVALID_HANDLE_VALUE)   
  {
    _hCom = NULL;
    return FALSE;
  }

  /* set timeouts */
  timeouts.ReadIntervalTimeout         = MAXDWORD;
  timeouts.ReadTotalTimeoutConstant    = 0;
  timeouts.ReadTotalTimeoutMultiplier  = 0;
  timeouts.WriteTotalTimeoutMultiplier = 0;
  timeouts.WriteTotalTimeoutConstant   = 0;
  if(SetCommTimeouts(_hCom, &timeouts) == FALSE)
    return FALSE;

  dcb.DCBlength = sizeof(DCB);
  if(GetCommState(_hCom, &dcb) == FALSE)
    return FALSE;

  BuildCommDCB("baud=115200 parity=N data=8 stop=1", &dcb);
  return SetCommState(_hCom, &dcb);
}  

void closeComPort(void)
{
  CloseHandle(_hCom);
}

void sendByte(unsigned char byte)
{
  DWORD numberOfBytesWritten;
  
  WriteFile(_hCom,
            &byte,
            1,
            &numberOfBytesWritten,
            0);
}

int receiveByte(unsigned char* byte)
{
  DWORD numberOfBytesRead;
  
  ReadFile(_hCom,
           byte,
           1,
           &numberOfBytesRead,
           0);

  return numberOfBytesRead;
}