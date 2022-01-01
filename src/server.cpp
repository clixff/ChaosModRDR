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
	while (true)
	{
		instance.run_one();

		if (!bStarted) return;
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

	if (eventType == "update-interval")
	{
		OnIntervalUpdated(document);
	}
	else if (eventType == "activate-effect")
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

void WebSocketServer::OnIntervalUpdated(rapidjson::Document &document)
{
	int32_t intervalTime = 0;
	int32_t votingTime = 0;

	try
	{
		if (!document.HasMember("interval") || !document.HasMember("voting"))
		{
			return;
		}

		intervalTime = document["interval"].GetInt();
		votingTime = document["voting"].GetInt();

	}
	catch (int err)
	{
		//
	}

	ChaosMod::globalMutex.lock();

	ChaosMod::Singleton->intervalsData.intervalTime = intervalTime;
	ChaosMod::Singleton->intervalsData.votingTime = votingTime;

	ChaosMod::globalMutex.unlock();
}

void WebSocketServer::OnNewEffectActivated(rapidjson::Document &document)
{
	EffectToActivate effect;

	try
	{
		if (!document.HasMember("id") || !document.HasMember("name") || !document.HasMember("duration"))
		{
			return;
		}

		effect.id = document["id"].GetString();
		effect.name = document["name"].GetString();
		effect.duration = document["duration"].GetInt();
	}
	catch (int err)
	{
		//
	}

	ChaosMod::globalMutex.lock();

	ChaosMod::Singleton->effectToActivate = effect;

	ChaosMod::globalMutex.unlock();
}