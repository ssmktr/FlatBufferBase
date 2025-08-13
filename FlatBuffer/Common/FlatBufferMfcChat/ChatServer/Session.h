#pragma once
#include "Message.h"

class CChatServer;

class CSession : public std::enable_shared_from_this<CSession>
{
public:
	CSession(boost::asio::ip::tcp::socket socket, CChatServer* pServer);
	~CSession();

public:
	boost::asio::ip::tcp::socket& Socket() { return m_Socket; }

	void Start();
	void WritePacket(const CMessage& msg);

	void SetName(const char* pszName) { m_Name = pszName; }
	const char* GetName() { return m_Name.c_str(); }

private:
	void handle_write();
	void handle_read_header();
	void handle_read_body();

private:
	boost::asio::ip::tcp::socket m_Socket;

	std::string m_Name;
	CChatServer* m_pServer;

	CMessage m_ReadMessage;
	std::deque<CMessage> m_WriteMessage;
};