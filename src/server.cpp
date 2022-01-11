#include "server.h"
#include "script.h"
#include <main.h>

WebSocketServer::WebSocketServer()
{

}

void WebSocketServer::Init(int32_t port)
{
	instance.set_open_handler(bind(&WebSocketServer::OnConnect, this, std::placeholders::_1));
	instance.set_close_handler(bind(&WebSocketServer::OnDisconnect, this, std::placeholders::_1));
	instance.set_message_handler(bind(&WebSocketServer::OnMessage, this, std::placeholders::_1, std::placeholders::_2));

	instance.set_access_channels(websocketpp::log::alevel::none);
	instance.set_error_channels(websocketpp::log::elevel::none);

	instance.init_asio();
	instance.listen("localhost", std::to_string(port));
	instance.start_accept();
}

void WebSocketServer::Run()
{
	bStarted = true;
	bool bJoinThread = false;
	while (true)
	{
		ChaosMod::globalMutex.lock();

		uint32_t lastTick = ChaosMod::LastTick;

		if (GetTickCount() > lastTick + 5000)
		{
			bool bGamePaused = UI::IS_PAUSE_MENU_ACTIVE();
			Ped ped = PLAYER::PLAYER_PED_ID();

			/** If game thread is sleeping, but game is not paused by player */
			bool bReloading = !bGamePaused;

			if (bReloading)
			{
				if (ChaosMod::PLAYER_PED)
				{
					bReloading = ChaosMod::PLAYER_PED != ped;
				}
				else
				{
					bReloading = false;
				}
			}

			if (bReloading)
			{
				Stop();
				bJoinThread = true;
				bStarted = false;
			}

			ChaosMod::LastTick = GetTickCount();
		}

		ChaosMod::globalMutex.unlock();

		instance.poll_one();

		if (!bStarted) break;
	}

	if (bJoinThread && ChaosMod::Singleton)
	{
		ChaosMod::globalMutex.lock();
		delete ChaosMod::Singleton->wsServer;
		ChaosMod::globalMutex.unlock();
	}
}

void WebSocketServer::OnMessage(websocketpp::connection_hdl hdl, _server::message_ptr msg)
{
	rapidjson::Document document;

	document.Parse(msg->get_payload().data());

	if (document.HasParseError())
	{
		return;
	}

	if (!document.HasMember("type"))
	{
		return;
	}

	std::string eventType = document["type"].GetString();

	if (eventType == "activate-effect")
	{
		OnNewEffectActivated(document);
	}
}

void WebSocketServer::OnConnect(websocketpp::connection_hdl hdl)
{
	if (this->client)
	{
		delete this->client;
	}

	this->client = new websocketpp::connection_hdl(hdl);
}

void WebSocketServer::OnDisconnect(websocketpp::connection_hdl hdl)
{
	if (this->client)
	{
		delete this->client;
		this->client = nullptr;
	}
}

void WebSocketServer::SendMessageToClient(std::string msg)
{
	this->SendMessageToClient(msg.c_str());
}

void WebSocketServer::SendMessageToClient(const char* msg)
{
	if (this->client)
	{
		instance.send(*this->client, msg, websocketpp::frame::opcode::value::text);
	}
}

void WebSocketServer::SendEffectNamesToClient(std::vector<std::string> names)
{
	rapidjson::Document document;

	document.SetObject();

	document.AddMember("type", "vote_activate", document.GetAllocator());

	rapidjson::Value namesArray;
	namesArray.SetArray();

	for (auto str : names)
	{
		rapidjson::Value str_;
		str_.SetString(str.c_str(), document.GetAllocator());
		namesArray.PushBack(str_, document.GetAllocator());
	}

	document.AddMember("data", namesArray, document.GetAllocator());

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	document.Accept(writer);

	auto str = buffer.GetString();

	//MessageBox(NULL, str, "Data", MB_OK);


	SendMessageToClient(buffer.GetString());
}

void WebSocketServer::Stop()
{
	bStarted = false;

	instance.stop();
	instance.stop_listening();
	instance.stop_perpetual();

	if (this->client)
	{
		delete this->client;
		this->client = nullptr;
	}
}

void WebSocketServer::OnNewEffectActivated(rapidjson::Document &document)
{
	int32_t winnerID = -1;

	try
	{
		if (!document.HasMember("index"))
		{
			return;
		}

		winnerID = document["index"].GetInt();
	}
	catch (int err)
	{
		//
	}

	ChaosMod::globalMutex.lock();

	ChaosMod::Singleton->twitchWinnerID = winnerID;

	ChaosMod::globalMutex.unlock();
}