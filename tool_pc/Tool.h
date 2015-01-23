#pragma once

class CCanvas;
class CUi;
class CChannelHandler;

class CTool
{
public:
	CTool(void);
	~CTool(void);
	CCanvas* getCanvas();
	CUi* getUi();
	CChannelHandler* getChannelHandler();
	void serialTimeTick();
	int getSerialTime();

private:
	CCanvas* m_pCanvas;
	CUi* m_pUi;
	CChannelHandler* m_pChannelHandler;
	int m_iSerialTime;
};
