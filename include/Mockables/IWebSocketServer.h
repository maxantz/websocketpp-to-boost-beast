#ifndef __I_WEB_SOCKET_SERVER_H__
#define __I_WEB_SOCKET_SERVER_H__

#include "IWebSocketConnection.h"
#include <functional>

namespace ws
{
	class IWebSocketServer
	{
	public:
		using MessageHandler = std::function<void(ConnectionPtr, const std::string&)>;

		virtual ~IWebSocketServer() = default;
		virtual void listen(uint16_t port) = 0;
		virtual void run() = 0;
		virtual void broadcast(std::string_view) = 0;
		virtual void setMessageHandler(MessageHandler) = 0;
	};
}

#endif	// __I_WEB_SOCKET_SERVER_H__
