#include <stdio.h>
#include <GL/freeglut.h>

#include "comport.h"

#include "Tool.h"
#include "Canvas.h"
#include "Ui.h"
#include "ChannelHandler.h"

static CTool* pTool;
static int serialTimeOld;

void display()
{
   pTool->getCanvas()->display();
   pTool->getUi()->checkKeyRepeat();
}

void idle()
{
   pTool->getChannelHandler()->readSerial();

   int serialTime = pTool->getSerialTime();
   int zoomout = pTool->getUi()->getZoomOut();

   if((serialTimeOld / zoomout) != (serialTime / zoomout))
   {
      glutPostRedisplay();
   }

   serialTimeOld = serialTime;
}

void keyboard(unsigned char key, int x, int y)
{
   pTool->getUi()->keyDown(key);
}

void keyboardUp(unsigned char key, int x, int y)
{
   pTool->getUi()->keyUp(key);
}

void mouse(int button, int state, int x, int y)
{
   pTool->getUi()->mouseButton(button, state);
}

void passiveMotion(int x, int y)
{
   pTool->getUi()->mouseMotion(x, y);
}

void reshape(int w, int h)
{
   pTool->getCanvas()->reshape(w, h);
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    if(argc < 2)
    {
        printf("Give serial port as parameter.\n");
        return -1;
    }

    if(!openComPort(argv[1]))
    {
        printf("Cannot open serial port %s.\n", argv[1]);
        return -1;
    }

    pTool = new CTool();

    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    glutCreateWindow("yabr measurement tool");
    glutFullScreen();

    glutIgnoreKeyRepeat(true);

    glutDisplayFunc(display);
    glutIdleFunc(idle);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(passiveMotion);

    sendByte('X');
    pTool->getUi()->sendPacketBegin('m');
    pTool->getUi()->sendPacketEnd();
	
    printf("enter mainloop\n");
    system("sleep 1");
    glutMainLoop();

    delete pTool;

    closeComPort();

    return 0;
}
