#ifndef __I_WEB_SOCKET_CONNECTION_H__
#define __I_WEB_SOCKET_CONNECTION_H__

#include <memory>
#include <string_view>

namespace ws
{
	class IWebSocketConnection
	{
	public:
		virtual ~IWebSocketConnection() = default;

		virtual void send(std::string_view msg) = 0;
		virtual void close() = 0;
	};

	using ConnectionPtr = std::shared_ptr<IWebSocketConnection>;
}

#endif	// __I_WEB_SOCKET_CONNECTION_H__
