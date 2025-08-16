#include "pch.h"
#include "Session.h"
#include "ChatServer.h"

CChatServer::CChatServer(const unsigned short PORT_NUMBER)
    : m_Acceptor(m_IoContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER))
	, async_work(boost::asio::make_work_guard(m_IoContext.get_executor()))
{
}

CChatServer::~CChatServer()
{
	m_SessionList.clear();
}

void CChatServer::Start()
{
    async_thread.create_thread(boost::bind(&boost::asio::io_context::run, &m_IoContext));

    std::cout << "서버 시작... (Port : " << m_Acceptor.local_endpoint().port() << ")" << std::endl;
	
    handle_accept();
}

void CChatServer::Stop()
{
    async_work.reset();
    m_IoContext.stop();
    async_thread.join_all();

    std::cout << "서버 종료..." << std::endl;
}

void CChatServer::EndSession(std::shared_ptr<CSession> pSession)
{
	std::cout << "클라이언트 접속 종료 (" << pSession->GetName() << ")" << std::endl;

	pSession->Socket().close();
	m_SessionList.erase(pSession);
}

void CChatServer::handle_accept()
{
	m_Acceptor.async_accept(
		[this](boost::system::error_code error, tcp::socket socket)
		{
			if (!error)
			{
				std::cout << "클라이언트 접속 성공" << std::endl;

				auto pSession = std::make_shared<CSession>(std::move(socket), this);

				m_SessionList.insert(pSession);

				pSession->Start();
			}
			else
			{
				std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
			}

			handle_accept();
		});
}

void CChatServer::WritePacketAll(const CMessage& msg)
{
	for (auto it : m_SessionList)
		it->WritePacket(msg);
}

void CChatServer::ProcessPacket(std::shared_ptr<CSession> pSession, const uint8_t* pData, size_t size)
{
	// 패킷이 완성되었는지 체크
	auto verifier = flatbuffers::Verifier(pData, size);

	if (fbs::VerifyRootBuffer(verifier))
	{
		auto root = fbs::GetRoot(pData);

		// 패킷 식별 후 처리
		switch (root->packet_type())
		{
			case fbs::Packet_C2S_CONNECT_REQ:	// 클라이언트 접속 요청
			{
				auto raw = static_cast<const fbs::C2S_CONNECT_REQ*>(root->packet());

				flatbuffers::FlatBufferBuilder builder;

				auto data = fbs::CreateS2C_CONNECT_RES(builder, true);
				auto packet = fbs::CreateRoot(builder, fbs::Packet_S2C_CONNECT_RES, data.Union());

				builder.Finish(packet);

				pSession->WritePacket(CMessage(builder.GetBufferPointer(), builder.GetSize()));
				pSession->SetName(raw->name()->c_str());

				std::cout << "클라이언트 로그인 성공 Name: " << pSession->GetName() << std::endl;


				builder.Clear();

				auto name = builder.CreateString(raw->name()->c_str());
				auto datas = fbs::CreateS2C_ENTER_RES(builder, name);
				auto packets = fbs::CreateRoot(builder, fbs::Packet_S2C_ENTER_RES, datas.Union());
				builder.Finish(packets);

				for (auto it : m_SessionList)
				{
					if(it != pSession)
						it->WritePacket(CMessage(builder.GetBufferPointer(), builder.GetSize()));
				}

				break;
			}

			case fbs::Packet_C2S_LEAVE_REQ:		// 방 퇴장 요청
			{
				flatbuffers::FlatBufferBuilder builder;

				auto name = builder.CreateString(pSession->GetName());
				auto data = fbs::CreateS2C_LEAVE_RES(builder, name);
				auto packet = fbs::CreateRoot(builder, fbs::Packet_S2C_LEAVE_RES, data.Union());

				builder.Finish(packet);

				WritePacketAll(CMessage(builder.GetBufferPointer(), builder.GetSize()));

				break;
			}

			case fbs::Packet_C2S_USERLIST_REQ:		// 유저 리스트 요청
			{
				vector<string> v;
				for (auto it : m_SessionList)
				{
					v.push_back(it->GetName());
				}

				flatbuffers::FlatBufferBuilder builder;

				auto vs = builder.CreateVectorOfStrings(v);
				auto data = fbs::CreateS2C_USERLIST_RES(builder, vs);
				auto packet = fbs::CreateRoot(builder, fbs::Packet_S2C_USERLIST_RES, data.Union());

				builder.Finish(packet);

				pSession->WritePacket(CMessage(builder.GetBufferPointer(), builder.GetSize()));

				break;
			}

			case fbs::Packet_C2S_CHAT_REQ:		// 채팅 요청
			{
				auto raw = static_cast<const fbs::C2S_CHAT_REQ*>(root->packet());

				flatbuffers::FlatBufferBuilder builder;

				auto name = builder.CreateString(pSession->GetName());
				auto msg = builder.CreateString(raw->message()->c_str());

				auto data = fbs::CreateS2C_CHAT_RES(builder, name, msg);
				auto packet = fbs::CreateRoot(builder, fbs::Packet_S2C_CHAT_RES, data.Union());

				builder.Finish(packet);

				WritePacketAll(CMessage(builder.GetBufferPointer(), builder.GetSize()));

				break;
			}

			default:
				break;
		}
	}
	else
	{
		throw std::runtime_error("Ill-formed message");
	}
}