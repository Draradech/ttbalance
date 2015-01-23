#include "ChannelHandler.h"

#include <GL/freeglut.h>

#include "comport.h"

#include "Tool.h"
#include "Canvas.h"
#include "Channel.h"
#include "Parameter.h"

CChannelHandler::CChannelHandler(CTool* pTool)
{
	m_pTool = pTool;
	for(int i = 0; i < NUMCHANNELS; ++i)
	{
		m_vChannel[i] = new CChannel(m_pTool);
		m_vChannel[i]->setId('0');
		// todo: set initial channel config in yabr
	}
	m_vChannel[0]->setColor(1, 0, 0);
	m_vChannel[1]->setColor(0, 1, 0);
	m_vChannel[2]->setColor(0, 1, 1);
	m_vChannel[3]->setColor(1, 1, 0);
	m_vChannel[4]->setColor(1, 0, 1);

	m_pParameter = new CParameter(m_pTool);
	m_pParameter->setId('0');

	m_iPause = 0;
}

CChannelHandler::~CChannelHandler(void)
{
}

void CChannelHandler::togglePause()
{
	m_iPause = !m_iPause;
}

void CChannelHandler::setId(unsigned char channel, unsigned char id)
{
	m_vChannel[channel]->setId(id);
}

void CChannelHandler::setParameterId(unsigned char id)
{
	m_pParameter->setId(id);
}

void CChannelHandler::drawGraphs(int width, int height)
{
	for(int i = 0; i < NUMCHANNELS; ++i)
	{
		m_vChannel[i]->drawGraph(width, height);
	}
}

void CChannelHandler::drawChannels(int channelHeight)
{
	for(int i = 0; i < NUMCHANNELS; ++i)
	{
		m_vChannel[i]->drawInfo();
		glTranslatef(0.0f, GLfloat(channelHeight), 0.0f);
	}
}

void CChannelHandler::drawParameter()
{
	m_pParameter->draw();
}

// recv and decode byte
ERecvState CChannelHandler::recv(unsigned char *byte)
{
    static bool decryptNext;
    unsigned char rawByte;
    
    if(!decryptNext)
    {
        if(receiveByte(&rawByte))
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
        if(receiveByte(&rawByte))
        {
            decryptNext = false;
            *byte = rawByte ^ 0x20;
            return eRecvData;
        }
    }
    
    return eRecvNothing;
}

void CChannelHandler::readSerial()
{
    static unsigned char bytesInPacket;
    static bool inPacket;
    static unsigned short uint16 = 0;
    static signed short int16 = 0;
    
    ERecvState state = eRecvNothing;
    unsigned char byte = 0;
    
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
                    if(byte != 't')
					{
						// no measurement packet
						inPacket = false;
					}
					else if(!m_iPause)
					{
						m_pTool->serialTimeTick();
					}
                }
                else
                {
					if(bytesInPacket % 2)
					{
						uint16 = ((unsigned short)byte) << 8;
					}
					else
					{
						uint16 |= (unsigned short)byte;
						int16 = (signed short)uint16;

						int index = bytesInPacket/2 - 1;
						if(index < NUMCHANNELS) m_vChannel[index]->setValue(int16);
						if(index == NUMCHANNELS) m_pParameter->setValue(int16);
					}
                }
                bytesInPacket++;
            }
            else if(state == eRecvEnd)
            {
                // packet complete
                inPacket = false;
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
}
