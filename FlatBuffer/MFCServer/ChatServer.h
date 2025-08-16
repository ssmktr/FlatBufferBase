#pragma once
#include "pch.h"
#include <set>

class CSession;

class CChatServer
{
public:
	CChatServer(const unsigned short PORT_NUMBER);
	~CChatServer();

public:
	void Start();
	void Stop();

	void EndSession(std::shared_ptr<CSession>);
	void ProcessPacket(std::shared_ptr<CSession>, const uint8_t*, size_t);

private:
	void handle_accept();
	void WritePacketAll(const CMessage&);

private:
	boost::asio::io_context m_IoContext;
	boost::asio::ip::tcp::acceptor m_Acceptor;

	boost::asio::executor_work_guard<boost::asio::io_context::executor_type> async_work;
	boost::thread_group async_thread;


	std::set<std::shared_ptr<CSession>> m_SessionList;
};

