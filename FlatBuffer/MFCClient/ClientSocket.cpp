#include "pch.h"
#include "ClientSocket.h"

ClientSocket::ClientSocket()
{
}

ClientSocket::~ClientSocket()
{
}

void ClientSocket::OnClose(int nErrorCode)
{
	ShutDown();
	Close();

	CSocket::OnClose(nErrorCode);

	AfxMessageBox(_T("There is no connection to the server."));
}

void ClientSocket::OnReceive(int nErrorCode)
{
	if (Receive(m_ReadMessage.data(), CMessage::header_length) > 0)
	{
		if (m_ReadMessage.decode_header())
		{
			if (Receive(m_ReadMessage.body(), m_ReadMessage.body_length()) > 0)
			{
				ProcessPacket(m_ReadMessage.body(), m_ReadMessage.body_length());
			}
		}
	}

	CSocket::OnReceive(nErrorCode);
}

void ClientSocket::WritePacket(const CMessage& msg)
{
	m_WriteMessage.push_back(msg);

	if (m_WriteMessage.empty() == false)
	{
		do
		{
			Send(m_WriteMessage.front().data(), m_WriteMessage.front().length());

			m_WriteMessage.pop_front();

		} while (m_WriteMessage.empty() == false);
	}
}

void ClientSocket::ProcessPacket(const uint8_t* pData, size_t size)
{
}