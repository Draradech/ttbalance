#pragma once

#include <sstream>

typedef enum
{
	NoOverlay,
	OverlayChannelOne,
	OverlayChannelTwo,
	OverlayChannelThree,
	OverlayChannelFour,
	OverlayChannelFive,
	OverlayParameter,
	NumberOfOverlays
} Overlay_t;

class CTool;

class CUi
{
public:
	CUi(CTool* pTool);
	~CUi(void);

	void keyDown(char key);
	void keyUp(char key);
	void mouseButton(int button, int state);
	void mouseMotion(int x, int y);
	void draw(int width, int height);
	int getZoomOut();
	void sendPacketBegin(unsigned char command);
	void sendPacketEnd(void);
	void sendData(unsigned char byte);

private:
	CTool* m_pTool;
	int m_iMouseX, m_iMouseY;
	char m_sBuffer[512];
	Overlay_t m_eActiveOverlay;
	int m_iZoomOut;
	int m_iSkip;
};
