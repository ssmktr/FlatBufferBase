#pragma once
#include "ChatRoom.h"

class ChatServer
{
public:
	ChatServer(const unsigned short PORT_NUMBER);
	virtual ~ChatServer();

public:
	void Start();
	void Stop();

	void EndSession(std::shared_ptr<Session> pSession);
	void ProcessPacket(std::shared_ptr<Session> pSession, const uint8_t* pData, size_t size);

private:
	void handle_accept();

private:
	boost::asio::io_context m_IoContext;
	boost::asio::ip::tcp::acceptor m_Acceptor;

	//boost::shared_ptr<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>> async_work;
	boost::asio::executor_work_guard<boost::asio::io_context::executor_type> async_work;
	boost::thread_group async_thread;

	std::set<shared_ptr<Session>> m_SessionList;
	ChatRoom m_ChatRoom;
};

