#pragma once
#include <afxsock.h>

class CClientSocket : public CSocket
{
public:
	CClientSocket();
	~CClientSocket();

public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);

public:
	void WritePacket(const CMessage& msg);
	void ProcessPacket(const uint8_t* pData, size_t size);

private:
	CMessage m_ReadMessage;
	std::deque<CMessage> m_WriteMessage;
};
