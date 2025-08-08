#pragma once
#include <set>
#include "Session.h"

class ChatRoom
{
private:
	enum { max_recent_msgs = 10 };

public:
	ChatRoom() {}
	virtual ~ChatRoom() { m_SessionSet.clear(); }

public:
	void Enter(std::shared_ptr<Session> pSession)
	{
		m_SessionSet.insert(pSession);
	}

	void Leave(std::shared_ptr<Session> pSession)
	{
		m_SessionSet.erase(pSession);
	}

	void Write(const CMessage& msg)
	{
		for (auto session : m_SessionSet)
			session->Write(msg);
	}

private:
	std::set<std::shared_ptr<Session>> m_SessionSet;
};

