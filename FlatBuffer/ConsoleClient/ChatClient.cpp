#include "pch.h"
#include "ChatClient.h"

ChatClient::ChatClient(boost::asio::io_context& io_context, const tcp::resolver::results_type& endpoints)
	: m_IoContext(io_context)
	, m_Socket(m_IoContext)
	, m_bIsLogin(false)
{
	handle_connect(endpoints);
}

ChatClient::~ChatClient()
{
}

void ChatClient::Close()
{
	boost::asio::post(m_IoContext, [this]() {m_Socket.close(); });
}

void ChatClient::Write(const CMessage& msg)
{
	boost::asio::post(m_IoContext, [this, msg]() 
		{
			m_WriteMessage.push_back(msg);

			if (!m_WriteMessage.empty())
			{
				handle_write();
			}
		});
}

void ChatClient::handle_write()
{
	boost::asio::async_write(m_Socket, boost::asio::buffer(m_WriteMessage.front().data(), m_WriteMessage.front().length()),
		[this](boost::system::error_code error, std::size_t) 
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
				m_Socket.close();
			}
		});
}

void ChatClient::handle_connect(const tcp::resolver::results_type& endpoints)
{
	boost::asio::async_connect(m_Socket, endpoints, [this](boost::system::error_code error, tcp::endpoint)
		{
			if (!error)
			{
				std::cout << "서버 접속 성공" << std::endl;
				std::cout << "이름을 입력하세요: ";

				handle_read_header();
			}
			else
			{
				std::cout << "서버 접속 실패. error No : " << error.value() << " error Message : " << error.message() << std::endl;
			}
		});
}

void ChatClient::handle_read_header()
{
	boost::asio::async_read(m_Socket, boost::asio::buffer(m_ReadMessage.data(), CMessage::header_length),
		[this](boost::system::error_code error, std::size_t)
		{
			if (!error && m_ReadMessage.decode_header())
			{
				handle_read_body();
			}
			else
			{
				if (error == boost::asio::error::eof)
				{
					std::cout << "클라이언트와 연결이 끊어졌습니다!" << std::endl;
				}
				else
				{
					std::cout << "error No : " << error.value() << " error Message: " << error.message() << std::endl;
				}

				m_Socket.close();
			}
		});
}

void ChatClient::handle_read_body()
{
	boost::asio::async_read(m_Socket, boost::asio::buffer(m_ReadMessage.body(), m_ReadMessage.body_length()),
		[this](boost::system::error_code error, std::size_t) 
		{
			if (!error)
			{
				ProcessPacket(m_ReadMessage.data(), m_ReadMessage.body_length());

				handle_read_header();
			}
			else
			{
				if (error == boost::asio::error::eof)
				{
					std::cout << "클라이언트와 연결이 끊어졌습니다!" << std::endl;
				}
				else
				{
					std::cout << "error No : " << error.value() << " error Message : " << error.message() << std::endl;
				}

				m_Socket.close();
			}
		});
}

void ChatClient::ProcessPacket(const uint8_t* pData, size_t size)
{
	auto verifier = flatbuffers::Verifier(pData, size);

	if (fbs::VerifyRootBuffer(verifier))
	{
		auto root = fbs::GetRoot(pData);

		switch (root->packet_type())
		{
		case fbs::Packet_S2C_CONNECT_RES:
		{
			auto raw = static_cast<const fbs::S2C_CONNECT_RES*>(root->packet());

			if (raw->b_success() == true)
			{
				SetLogin(true);

				flatbuffers::FlatBufferBuilder builder;

				auto data = fbs::CreateC2S_ENTER_REQ(builder);
				auto packet = fbs::CreateRoot(builder, fbs::Packet_C2S_ENTER_REQ, data.Union());

				builder.Finish(packet);

				Write(CMessage(builder.GetBufferPointer(), builder.GetSize()));
			}
			else
			{
				std::cout << "클라이언트 로그인 실패!" << std::endl;
			}
		}
			break;

		case fbs::Packet_S2C_ENTER_RES:
		{
			auto raw = static_cast<const fbs::S2C_ENTER_RES*>(root->packet());

			std::cout << raw->name()->c_str() << " 님이 접속 하셨습니다." << std::endl;
		}
			break;

		case fbs::Packet_S2C_LEAVE_RES:
		{
			auto raw = static_cast<const fbs::S2C_LEAVE_RES*>(root->packet());

			std::cout << raw->name()->c_str() << " 님이 퇴장 하셨습니다." << std::endl;
		}
			break;

		case fbs::Packet_S2C_CHAT_RES:
		{
			auto raw = static_cast<const fbs::S2C_CHAT_RES*>(root->packet());
			std::cout << raw->name()->c_str() << " : " << raw->message()->c_str() << std::endl;
		}
			break;
		}
	}
	else
	{
		throw std::runtime_error("|||-formed message");
	}
}