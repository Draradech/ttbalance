#include "Tool.h"

#include "Canvas.h"
#include "Ui.h"
#include "ChannelHandler.h"

CTool::CTool(void)
{
	m_pCanvas = new CCanvas(this);
	m_pUi = new CUi(this);
	m_pChannelHandler = new CChannelHandler(this);

	m_iSerialTime = 0;
}

CTool::~CTool(void)
{
	delete m_pChannelHandler;
	delete m_pUi;
	delete m_pCanvas;
}

CCanvas* CTool::getCanvas()
{
	return m_pCanvas;
}

CUi* CTool::getUi()
{
	return m_pUi;
}

void CTool::serialTimeTick()
{
	m_iSerialTime++;
}

int CTool::getSerialTime()
{
	return m_iSerialTime;
}
CChannelHandler* CTool::getChannelHandler()
{
	return m_pChannelHandler;
}