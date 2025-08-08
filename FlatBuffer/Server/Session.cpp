#include "pch.h"
#include "Session.h"
#include "ChatServer.h"

Session::Session(boost::asio::ip::tcp::socket socket, ChatServer* pServer)
	: m_Socket(std::move(socket))
	, m_pServer(pServer)
{
}

Session::~Session()
{
}

void Session::Start()
{
	handle_read_header();
}

void Session::Write(const CMessage& msg)
{
	m_WriteMessage.push_back(msg);

	if (m_WriteMessage.empty() == false)
	{
		handle_write();
	}
}

void Session::handle_write()
{
	auto self(shared_from_this());
	boost::asio::async_write(m_Socket, boost::asio::buffer(m_WriteMessage.front().data(), m_WriteMessage.front().length()),
		[this, self](boost::system::error_code error, std::size_t)
		{
			if (!error)
			{
				m_WriteMessage.pop_front();
				if (!m_WriteMessage.empty())
				{
					handle_write();
				}
			}
			else
			{
				m_pServer->EndSession(shared_from_this());
			}
		});
}

void Session::handle_read_header()
{
	auto self(shared_from_this());
	boost::asio::async_read(m_Socket, boost::asio::buffer(m_ReadMessage.data(), CMessage::header_length),
		[this, self](boost::system::error_code error, std::size_t)
		{
			if (!error && m_ReadMessage.decode_header())
			{
				handle_read_body();
			}
			else
			{
				m_pServer->EndSession(shared_from_this());
			}
		});
}

void Session::handle_read_body()
{
	auto self(shared_from_this());
	boost::asio::async_read(m_Socket, boost::asio::buffer(m_ReadMessage.data(), m_ReadMessage.body_length()),
		[this, self](boost::system::error_code error, std::size_t)
		{
			if (!error)
			{
				m_pServer->ProcessPacket(shared_from_this(), m_ReadMessage.body(), m_ReadMessage.body_length());

				handle_read_header();
			}
			else
			{
				if (error == boost::asio::error::eof)
				{
					std::cout << "클라이언트와 연결이 끊어졌습니다." << std::endl;
				}
				else
				{
					std::cout << "error No : " << error.value() << " error Message : " << error.message() << std::endl;
				}

				m_pServer->EndSession(shared_from_this());
			}
		});
}
