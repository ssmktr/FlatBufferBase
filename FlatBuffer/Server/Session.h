#pragma once
#include "Message.h"

class ChatServer;

class Session : public std::enable_shared_from_this<Session>
{
public:
	Session(boost::asio::ip::tcp::socket socket, ChatServer* pServer);
	virtual ~Session();

public:
	boost::asio::ip::tcp::socket& Socket() { return m_Socket; }

	void Start();
	void Write(const CMessage& msg);

	void SetName(const char* pszName) { m_Name = pszName; }
	const char* GetName() { return m_Name.c_str(); }

private:
	void handle_write();
	void handle_read_header();
	void handle_read_body();

private:
	boost::asio::ip::tcp::socket m_Socket;

	std::string m_Name;
	ChatServer* m_pServer;

	CMessage m_ReadMessage;
	std::deque<CMessage> m_WriteMessage;
};

