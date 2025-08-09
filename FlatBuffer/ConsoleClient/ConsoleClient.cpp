#include "pch.h"
#include "ChatClient.h"

#define HOST_IP "127.0.0.1"
#define PORT_NUMBER "31400"

int main()
{
	try
	{
		boost::asio::io_context io_context;
		tcp::resolver resolver(io_context);

		ChatClient client(io_context, resolver.resolve(HOST_IP, PORT_NUMBER));
		std::thread t([&io_context]() {io_context.run(); });

		char szInput[CMessage::max_body_length + 1] = { 0, };
		while (std::cin.getline(szInput, CMessage::max_body_length + 1))
		{
			if (strnlen_s(szInput, CMessage::max_body_length + 1) == 0)
			{
				flatbuffers::FlatBufferBuilder builder;

				auto data = fbs::CreateC2S_LEAVE_REQ(builder);
				auto root = fbs::CreateRoot(builder, fbs::Packet_C2S_LEAVE_REQ, data.Union());

				builder.Finish(root);

				client.Write(CMessage(builder.GetBufferPointer(), builder.GetSize()));

				Sleep(500);
				break;
			}

			if (client.IsConnect() == false)
			{
				std::cout << "서버와 연결되지 않았습니다!" << std::endl;
				continue;
			}

			if (client.GetLogin() == false)
			{
				flatbuffers::FlatBufferBuilder builder;

				auto name = builder.CreateString(szInput);
				auto data = fbs::CreateC2S_CONNECT_REQ(builder, name);
				auto root = fbs::CreateRoot(builder, fbs::Packet_C2S_CONNECT_REQ, data.Union());

				builder.Finish(root);

				client.Write(CMessage(builder.GetBufferPointer(), builder.GetSize()));
			}
			else
			{
				flatbuffers::FlatBufferBuilder builder;

				auto messag = builder.CreateString(szInput);
				auto data = fbs::CreateC2S_CHAT_REQ(builder, messag);
				auto root = fbs::CreateRoot(builder, fbs::Packet_C2S_CHAT_REQ, data.Union());

				builder.Finish(root);

				client.Write(CMessage(builder.GetBufferPointer(), builder.GetSize()));
			}
		}

		io_context.stop();
		client.Close();
		t.join();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << std::endl;
	}

	return 0;
}
