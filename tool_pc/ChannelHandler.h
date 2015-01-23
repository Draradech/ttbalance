#pragma once

#define NUMCHANNELS 5

class CTool;
class CChannel;
class CParameter;

typedef enum
{
    eRecvNothing = 0,
    eRecvData,
    eRecvBegin,
    eRecvEnd
} ERecvState;

class CChannelHandler
{
public:
	CChannelHandler(CTool* pTool);
	~CChannelHandler(void);
	void readSerial();
	void setId(unsigned char channel, unsigned char id);
	void setParameterId(unsigned char id);
	void drawGraphs(int width, int height);
	void drawChannels(int channelHeight);
	void drawParameter();
	void togglePause();

private:
	CTool* m_pTool;
	CChannel* m_vChannel[NUMCHANNELS];
	CParameter* m_pParameter;
	ERecvState recv(unsigned char *byte);

	int m_iPause;
};
