#pragma once
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

	boost::shared_ptr<boost::asio::io_service::work> async_work;
	boost::thread_group async_thread;


	std::set<std::shared_ptr<CSession>> m_SessionList;
};

