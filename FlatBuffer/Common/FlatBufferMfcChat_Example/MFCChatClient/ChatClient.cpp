#include "pch.h"
#include "ChatClient.h"

#include "MFCChatClientDlg.h"


CClientSocket::CClientSocket()
{
}

CClientSocket::~CClientSocket()
{
}


void CClientSocket::OnClose(int nErrorCode)
{
	ShutDown();
	Close();

	CSocket::OnClose(nErrorCode);

	AfxMessageBox(_T("There is no connection to the server."));
}

void CClientSocket::OnReceive(int nErrorCode)
{
	if (Receive(m_ReadMessage.data(), CMessage::header_length) > 0)
	{
		if (m_ReadMessage.decode_header())
		{
			if (Receive(m_ReadMessage.body(), m_ReadMessage.body_length()) > 0)
			{
				ProcessPacket(m_ReadMessage.body(), m_ReadMessage.body_length());
			}
		}
	}

	CSocket::OnReceive(nErrorCode);
}

void CClientSocket::WritePacket(const CMessage& msg)
{
	m_WriteMessage.push_back(msg);

	if (!m_WriteMessage.empty())
	{
		do
		{
			Send(m_WriteMessage.front().data(), m_WriteMessage.front().length());

			m_WriteMessage.pop_front();
		}
		while (!m_WriteMessage.empty());
	}
}


void CClientSocket::ProcessPacket(const uint8_t* pData, size_t size)
{
	CMFCChatClientDlg* pMainDlg = (CMFCChatClientDlg*)AfxGetMainWnd();
	auto verifier = flatbuffers::Verifier(pData, size);

	if (fbs::VerifyRootBuffer(verifier))
	{
		auto root = fbs::GetRoot(pData);

		switch (root->packet_type())
		{
		case fbs::Packet_S2C_CONNECT_RES:	// 연결 응답
		{
			auto raw = static_cast<const fbs::S2C_CONNECT_RES*>(root->packet());

			if (raw->b_success() == true)
			{
				pMainDlg->AddChat(_T("서버에 접속하였습니다"));

				flatbuffers::FlatBufferBuilder builder;

				auto data = fbs::CreateC2S_USERLIST_REQ(builder);
				auto packet = fbs::CreateRoot(builder, fbs::Packet_C2S_USERLIST_REQ, data.Union());

				builder.Finish(packet);

				WritePacket(CMessage(builder.GetBufferPointer(), builder.GetSize()));
			}
			else
			{
				pMainDlg->AddChat(_T("서버 접속 실패!"));
			}

			break;
		}

		case fbs::Packet_S2C_ENTER_RES:		// 입장 응답
		{
			auto raw = static_cast<const fbs::S2C_ENTER_RES*>(root->packet());
			
			CString str;
			str = raw->name()->c_str();
			str += _T(" 님이 접속하였습니다.");
			
			pMainDlg->AddChat(str);
			pMainDlg->AddUser(CString(raw->name()->c_str()).GetString());

			break;
		}

		case fbs::Packet_S2C_LEAVE_RES:		// 퇴장 응답
		{
			auto raw = static_cast<const fbs::S2C_LEAVE_RES*>(root->packet());

			CString str;
			str = raw->name()->c_str();
			str += _T(" 님이 퇴장하였습니다.");

			pMainDlg->AddChat(str);
			pMainDlg->DelUser(CString(raw->name()->c_str()).GetString());

			break;
		}

		case fbs::Packet_S2C_USERLIST_RES:	// 유저 리스트 응답
		{
			auto raw = static_cast<const fbs::S2C_USERLIST_RES*>(root->packet());
			
			for (auto it = raw->name()->begin(); it != raw->name()->end(); it++)
			{
				pMainDlg->AddUser((*it)->c_str());
			}

			break;
		}

		case fbs::Packet_S2C_CHAT_RES:		// 채팅 응답
		{
			auto raw = static_cast<const fbs::S2C_CHAT_RES*>(root->packet());

			CString str;
			str.Format(_T("%s: %s"), raw->name()->c_str(), raw->message()->c_str());
			
			pMainDlg->AddChat(str);

			break;
		}
		}
	}
	else
	{
		AfxMessageBox(_T("Ill-formed message"));
	}
}
