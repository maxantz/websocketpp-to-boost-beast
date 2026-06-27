#pragma once
#pragma once

#include "Mockables/IWebSocketServer.h"

#include "BeastConnection.hpp"
#include <boost/asio.hpp>
#include <thread>
#include <vector>
#include <unordered_set>
#include <shared_mutex>

namespace ws
{
	class BeastServer : public IWebSocketServer
	{
	public:
		BeastServer(size_t threads =
			std::thread::hardware_concurrency())
			: threads_(threads),
			  acceptor_(ioc_)
		{}

		void listen(uint16_t port)
		{
			boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);

			acceptor_.open(endpoint.protocol());
			acceptor_.set_option(
				boost::asio::socket_base::reuse_address(true));

			acceptor_.bind(endpoint);
			acceptor_.listen();

			doAccept();
		}

		void run()
		{
			for(size_t i=0;i<threads_;++i)
			{
				pool_.emplace_back([this]
				{
					ioc_.run();
				});
			}

			for(auto& t:pool_)
				t.join();
		}

		void broadcast(std::string_view msg)
		{
			std::shared_lock lock(mutex_);

			for(auto& c:connections_)
				c->send(msg);
		}

		void setMessageHandler(MessageHandler h)
		{
			messageHandler_ = std::move(h);
		}

	private:

		void doAccept()
		{
			auto conn =
				std::make_shared<BeastConnection>(ioc_);

			acceptor_.async_accept(
				conn->socket().next_layer(),
				[this,conn](auto ec)
				{
					if(!ec)
					{
						conn->setMessageHandler(messageHandler_);

						{
							std::unique_lock lock(mutex_);
							connections_.insert(conn);
						}

						conn->start();
					}

					doAccept();
				});
		}

	private:

		boost::asio::io_context ioc_;
		size_t threads_;

		boost::asio::ip::tcp::acceptor acceptor_;

		std::vector<std::thread> pool_;

		std::unordered_set<ConnectionPtr> connections_;
		std::shared_mutex mutex_;

		MessageHandler messageHandler_;
	};
}
