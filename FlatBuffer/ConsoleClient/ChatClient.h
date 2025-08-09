#pragma once
#include "pch.h"

class ChatClient
{
public:
	ChatClient(boost::asio::io_context& io_context, const tcp::resolver::results_type& endpoints);
	virtual ~ChatClient();

public:
	void SetLogin(bool bLogin) { m_bIsLogin = bLogin; }
	bool GetLogin() { return m_bIsLogin; }

	bool IsConnect() { return m_Socket.is_open(); }

	void Close();
	void Write(const CMessage& msg);

	void ProcessPacket(const uint8_t* pData, size_t size);

private:
	void handle_write();
	void handle_connect(const tcp::resolver::results_type& endpoints);
	void handle_read_header();
	void handle_read_body();

private:
	bool m_bIsLogin = true;

	boost::asio::io_context& m_IoContext;
	boost::asio::ip::tcp::socket m_Socket;

	CMessage m_ReadMessage;
	std::deque<CMessage> m_WriteMessage;
};

