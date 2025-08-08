// ChatServer.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

/*-------------------------------------------------------------------------
Boost Library를 설치하셔야 됩니다.

Nuget 패키지 설치를 이용하여 아래 4가지를 추가하세요

boost
boost_chrono-vc142
boost_date_time-vc142
boost_thread-vc142

설명: https://copynull.tistory.com/410?category=194491 [Thinking Different]
--------------------------------------------------------------------------*/

#include "stdafx.h"
#include "ChatServer.h"

const unsigned short PORT_NUMBER = 31400;

int main()
{
	CChatServer server(PORT_NUMBER);

	server.Start();
	
	getchar();
	
	server.Stop();

    return 0;
}