#include "stdinc.h"

#define SERIALBUFFER 64
#define DATABUFFER   32

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//   B U F F E R S                                                           //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

typedef enum
{
   eRecvNothing = 0,
   eRecvData,
   eRecvBegin,
   eRecvEnd
} ERecvState;

typedef struct
{
   volatile uint8_t indexIn;
   volatile uint8_t indexOut;
   volatile uint8_t buffer[SERIALBUFFER];
} SCircularBuffer;

static volatile SCircularBuffer sendBuffer;
static volatile SCircularBuffer recvBuffer;

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//   H A N D L E   B U F F E R S                                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

static force_inline uint8_t length(volatile SCircularBuffer *buf)
{
   return (uint8_t)((buf->indexIn % SERIALBUFFER) - (buf->indexOut % SERIALBUFFER) + SERIALBUFFER) % SERIALBUFFER;
}

static force_inline uint8_t deque(volatile SCircularBuffer *buf)
{
   uint8_t val = buf->buffer[buf->indexOut];
   buf->indexOut = (uint8_t)(buf->indexOut + 1) % SERIALBUFFER;
   return val;
}

static force_inline void enque(volatile SCircularBuffer *buf, uint8_t val)
{
   buf->buffer[buf->indexIn] = val;
   buf->indexIn = (uint8_t)(buf->indexIn + 1) % SERIALBUFFER;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//   I N T E R R U P T S                                                     //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// ready to send interrupt
ISR(USART0_UDRE_vect)
{
   if(length(&sendBuffer) > 0)
   {
      // send next byte
      UDR0 = deque(&sendBuffer);
   }
   else
   {
      // deactivate interrupt
      UCSR0B &= ~(1 << UDRIE0);
   }
}

// byte recieved interrupt
ISR(USART0_RX_vect)
{
   if(length(&recvBuffer) < (SERIALBUFFER - 1))
   {
      // put byte in recv buffer
      enque(&recvBuffer, UDR0);
   }
   else
   {
      // discard byte
      uint8_t discard;
      discard = UDR0;
   }
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//   L O W L E V E L                                                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// send byte
static void sendRaw(uint8_t byte)
{
   if(length(&sendBuffer) < (SERIALBUFFER - 1))
   {
      // put byte in send buffer, enable interrupt
      enque(&sendBuffer, byte);
      UCSR0B |= (1 << UDRIE0);
   }
   else
   {
      // discard
   }
}

// recv byte
static bool recvRaw(uint8_t *byte)
{
   if(length(&recvBuffer) > 0)
   {
      // pull byte out of recv buffer
      *byte = deque(&recvBuffer);
      return true;
   }
   return false;
}

// recv and decode byte
static ERecvState recv(uint8_t *byte)
{
   static bool decryptNext;
   uint8_t rawByte;
   
   if(!decryptNext)
   {
      if(recvRaw(&rawByte))
      {
         if(rawByte == '}')
         {
            return eRecvEnd;
         }
         else if(rawByte == '{')
         {
            return eRecvBegin;
         }
         else if(rawByte == '|')
         {
            decryptNext = true;
         }
         else
         {
            *byte = rawByte;
            return eRecvData;
         }
      }
   }
   
   if(decryptNext)
   {
      if(recvRaw(&rawByte))
      {
         decryptNext = false;
         *byte = rawByte ^ 0x20;
         return eRecvData;
      }
   }
   
   return eRecvNothing;
}

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//   H I G H L E V E L                                                       //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

// start packet
void sendPacketBegin(uint8_t command)
{
   sendRaw('{');
   sendData(command);
}

// end packet
void sendPacketEnd(void)
{
   sendRaw('}');
}

// send data byte
void sendData(uint8_t byte)
{
   if (  (byte == '|')   // escape
      || (byte == '{')   // packet begin
      || (byte == '}')   // packet end
      || (byte == '\0')  // shouldn't be a problem, but who knows...
      || (byte == '$')   // do not enter bluetooth command mode accidentally
      )
   {
      sendRaw('|');
      byte = byte ^ 0x20;
   }
   sendRaw(byte);
}

void sendData16(uint16_t word)
{
   sendData(HIGH(word));
   sendData(LOW(word));
}

bool recvPacket(uint8_t *command, uint8_t *size, uint8_t **dataout)
{
   static uint8_t data[DATABUFFER];
   static uint8_t commandSave;
   static uint8_t bytesInPacket;
   static bool inPacket;
   
   ERecvState state = eRecvNothing;
   uint8_t byte = 0;
   
   while((state = recv(&byte)) != eRecvNothing)
   {
      if(inPacket)
      {
         // in packet
         if(state == eRecvBegin)
         {
            // incomplete packet in buffer, discard
            bytesInPacket = 0;
         }
         else if(state == eRecvData)
         {
            if(bytesInPacket == 0)
            {
               commandSave = byte;
            }
            else if(bytesInPacket - 1 < DATABUFFER)
            {
               data[bytesInPacket - 1] = byte;
            }
            else
            {
               // packet too long for buffer, discard
               inPacket = false;
            }
            bytesInPacket++;
         }
         else if(state == eRecvEnd)
         {
            // packet complete, deliver
            inPacket = false;
            *size = bytesInPacket - 1;
            *command = commandSave;
            *dataout = data;
            return true;
         }
      }
      else
      {
         // outside packet
         if(state == eRecvBegin)
         {
            inPacket = true;
            bytesInPacket = 0;
         }
         else if(state == eRecvData)
         {
            ; // ignore
         }
         else if(state == eRecvEnd)
         {
            ; // ignore
         }
      }
   }
   
   return false;
}
