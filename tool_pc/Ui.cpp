#include "Ui.h"

#include <stdio.h>
#include <GL/freeglut.h>
#include "Tool.h"
#include "ChannelHandler.h"
#include "comport.h"
#include "MeasurementTable.h"

CUi::CUi(CTool* pTool)
{
	m_pTool = pTool;
	m_eActiveOverlay = NoOverlay;
	m_iZoomOut = 1;
	m_iSkip = 1;
}

CUi::~CUi(void)
{
}

void CUi::keyDown(char key)
{
	switch(m_eActiveOverlay)
	{
		case NoOverlay:
		{
			switch(key)
			{
				case 27:
				{
					glutLeaveMainLoop();
					break;
				}
				case '1':
				case '2':
				case '3':
				case '4':
				case '5':
				{
					m_eActiveOverlay = (Overlay_t) (OverlayChannelOne + (key - '1'));
					break;
				}
				case 'p':
				{
					m_eActiveOverlay = OverlayParameter;
					break;
				}
				case ' ':
				{
					sendPacketBegin('x');
                                        sendPacketEnd();
					break;
				}
				case 'i':
				{
					if(m_iZoomOut > 1) m_iZoomOut--;
					break;
				}
				case 'o':
				{
					if(m_iZoomOut < 10) m_iZoomOut++;
					break;
				}
				case 'w':
				{
					sendPacketBegin('f');
					sendData(75);
					sendPacketEnd();
					break;
				}
				case 's':
				{
					sendPacketBegin('f');
					sendData(-75);
					sendPacketEnd();
					break;
				}
				case 'a':
				{
					sendPacketBegin('l');
					sendData(-75);
					sendPacketEnd();
					break;
				}
				case 'd':
				{
					sendPacketBegin('l');
					sendData(75);
					sendPacketEnd();
					break;
				}
				// todo: check bounds
				case '+':
				{
					sendPacketBegin('i');
					sendData(1);
					sendPacketEnd();
					break;
				}
				case '*':
				{
					sendPacketBegin('i');
					sendData(10);
					sendPacketEnd();
					break;
				}
				case '-':
				{
					sendPacketBegin('i');
					sendData(-1);
					sendPacketEnd();
					break;
				}
				case '_':
				{
					sendPacketBegin('i');
					sendData(-10);
					sendPacketEnd();
					break;
				}
				case 'r':
				{
					sendPacketBegin('r');
					sendPacketEnd();
					// todo: resend current channel config
					break;
				}
				default:
				{
					break;
				}
			}
			break;
		}
		case OverlayChannelOne:
		case OverlayChannelTwo:
		case OverlayChannelThree:
		case OverlayChannelFour:
		case OverlayChannelFive:
		{
			switch(key)
			{
				case 27:
				{
					m_eActiveOverlay = NoOverlay;
					break;
				}
				default:
				{
					// todo: if in range of measurement values, choose -> else show message
					sendPacketBegin('c');
					sendData(m_eActiveOverlay - OverlayChannelOne);
					sendData(key);
					sendPacketEnd();
					m_pTool->getChannelHandler()->setId(m_eActiveOverlay - OverlayChannelOne, key);
					m_eActiveOverlay = NoOverlay;
					break;
				}
			}
			break;
		}
		case OverlayParameter:
		{
			switch(key)
			{
				case 27:
				{
					m_eActiveOverlay = NoOverlay;
					break;
				}
				default:
				{
					// todo: if in range...
					sendPacketBegin('p');
					sendData(key);
					sendPacketEnd();
					m_pTool->getChannelHandler()->setParameterId(key);
					m_eActiveOverlay = NoOverlay;
					break;
				}
			}
			break;
		}
	}
	glutPostRedisplay();
}

void CUi::keyUp(char key)
{
}

void CUi::mouseButton(int button, int state)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
	}
}

void CUi::mouseMotion(int x, int y)
{
	m_iMouseX = x;
	m_iMouseY = y;
}

void CUi::draw(int width, int height)
{
	switch(m_eActiveOverlay)
	{
		case NoOverlay:
		{
			break;
		}
		case OverlayChannelOne:
		case OverlayChannelTwo:
		case OverlayChannelThree:
		case OverlayChannelFour:
		case OverlayChannelFive:
		{
			glColor3f(0.0f, 0.0f, 0.0f);
			glRectf(GLfloat(width / 10 + 1), GLfloat(height / 10 + 1), GLfloat(9 * width / 10), GLfloat(9 * height / 10 - 1));

			sprintf(m_sBuffer, "Messwertauswahl Kanal %d", m_eActiveOverlay - OverlayChannelOne + 1);
			glColor3f(1.0f, 1.0f, 1.0f);
			glRasterPos2f(GLfloat(width / 10 + 1 + 40), GLfloat(height / 10 + 1 + 14 + 40));
			glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)m_sBuffer);

			for(int i = 0; measurementTable[i].id; ++i)
			{
				sprintf(m_sBuffer, "%c: %s", measurementTable[i].id, measurementTable[i].name.c_str());
                glColor3f(1.0f, 1.0f, 1.0f);
				glRasterPos2f(GLfloat(width / 10 + 1 + 40), GLfloat(height / 10 + 1 + 14 + 40 + 24 * (i + 2)));
				glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)m_sBuffer);
			}

			break;
		}
		case OverlayParameter:
		{
			glColor3f(0.0f, 0.0f, 0.0f);
			glRectf(GLfloat(width / 10 + 1), GLfloat(height / 10 + 1), GLfloat(9 * width / 10), GLfloat(9 * height / 10 - 1));

			sprintf(m_sBuffer, "Parameterauswahl");
			glColor3f(1.0f, 1.0f, 1.0f);
			glRasterPos2f(GLfloat(width / 10 + 1 + 40), GLfloat(height / 10 + 1 + 14 + 40));
			glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)m_sBuffer);

			for(int i = 0; parameterTable[i].id; ++i)
			{
				sprintf(m_sBuffer, "%c: %s", parameterTable[i].id, parameterTable[i].name.c_str());
                glColor3f(1.0f, 1.0f, 1.0f);
				glRasterPos2f(GLfloat(width / 10 + 1 + 40), GLfloat(height / 10 + 1 + 14 + 40 + 24 * (i + 2)));
				glutBitmapString(GLUT_BITMAP_HELVETICA_18, (unsigned char*)m_sBuffer);
			}

			break;
		}
	}
}

//begin packet
void CUi::sendPacketBegin(unsigned char command)
{
    sendByte('{');
    sendData(command);
}

// end packet
void CUi::sendPacketEnd(void)
{
    sendByte('}');
}

// send data byte
void CUi::sendData(unsigned char byte)
{
    if (  (byte == '|')    // escape
       || (byte == '{')    // packet begin
       || (byte == '}')    // packet end
       || (byte == '\0')   // shouldn't be a problem, but who knows...
       || (byte == '$')    // do not enter bluetooth command mode accidentally
       )
    {
        sendByte('|');
        byte = byte ^ 0x20;
    }
    sendByte(byte);
}

int CUi::getZoomOut()
{
	return m_iZoomOut;
}
