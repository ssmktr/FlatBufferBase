#include "pch.h"
#include "ChatServer.h"

ChatServer::ChatServer(const unsigned short PORT_NUMBER)
	: m_Acceptor(m_IoContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER))
	//, async_work(boost::make_shared<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>>)
	, async_work(boost::asio::make_work_guard(m_IoContext.get_executor()))
{
}

ChatServer::~ChatServer()
{
	m_SessionList.clear();
}

void ChatServer::Start()
{
	async_thread.create_thread(boost::bind(&boost::asio::io_context::run, &m_IoContext));

	std::cout << "서버 시작... (Port : " << m_Acceptor.local_endpoint().port() << ")" << std::endl;

	handle_accept();
}

void ChatServer::Stop()
{
	async_work.reset();
	m_IoContext.stop();
	async_thread.join_all();

	std::cout << "서버 종료..." << std::endl;
}

void ChatServer::EndSession(std::shared_ptr<Session> pSession)
{
	std::cout << "클라이언트 접속 종료 (" << pSession->GetName() << ")" << std::endl;

	m_ChatRoom.Leave(pSession);
	pSession->Socket().close();
	m_SessionList.erase(pSession);
}

void ChatServer::handle_accept()
{
	m_Acceptor.async_accept([this](boost::system::error_code error, tcp::socket socket)
		{
			if (!error)
			{
				std::cout << "클라이언트 접속 성공!!" << std::endl;

				auto pSession = std::make_shared<Session>(std::move(socket), this);

				m_SessionList.insert(pSession);

				pSession->Start();
			}
			else
			{
				std::cout << "error No : " << error.value() << " error Message : " << error.message() << std::endl;
			}

			handle_accept();
		});
}

void ChatServer::ProcessPacket(std::shared_ptr<Session> pSession, const uint8_t* pData, size_t size)
{
	auto verifier = flatbuffers::Verifier(pData, size);

	if (fbs::VerifyRootBuffer(verifier))
	{
		auto root = fbs::GetRoot(pData);

		switch (root->packet_type())
		{
		case fbs::Packet_C2S_CONNECT_REQ:
		{
			auto raw = static_cast<const fbs::C2S_CONNECT_REQ*>(root->packet());

			flatbuffers::FlatBufferBuilder builder;

			auto data = fbs::CreateS2C_CONNECT_RES(builder, true);
			auto packet = fbs::CreateRoot(builder, fbs::Packet_S2C_CONNECT_RES, data.Union());

			builder.Finish(packet);

			pSession->Write(CMessage(builder.GetBufferPointer(), builder.GetSize()));
			pSession->SetName(raw->name()->c_str());

			std::cout << "클라이언트 로그인 성공 Name : " << pSession->GetName() << std::endl;
		}
			break;
		}
	}
}

