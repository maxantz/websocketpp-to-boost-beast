#ifndef __BEAST_CONNECTION_HPP__
#define __BEAST_CONNECTION_HPP__

#include "Mockables/IWebSocketConnection.h"

#include <boost/beast/websocket.hpp>
#include <boost/beast/core.hpp>
#include <boost/asio.hpp>

#include <deque>

namespace ws
{
	class BeastConnection :
		public IWebSocketConnection,
		public std::enable_shared_from_this<BeastConnection>
	{
	public:

		using tcp = boost::asio::ip::tcp;
		using websocket = boost::beast::websocket::stream<tcp::socket>;

		BeastConnection(boost::asio::io_context& ioc)
			: ws_(boost::asio::make_strand(ioc))
		{}

		websocket& socket()
		{
			return ws_;
		}

		void start()
		{
			ws_.set_option(
				boost::beast::websocket::stream_base::timeout::suggested(
					boost::beast::role_type::server));

			ws_.async_accept(
				boost::beast::bind_front_handler(
					&BeastConnection::onAccept,
					shared_from_this()));
		}

		void send(std::string_view msg) override
		{
			boost::asio::post(
				ws_.get_executor(),
				boost::beast::bind_front_handler(
					&BeastConnection::enqueueWrite,
					shared_from_this(),
					std::string(msg)));
		}

		void close() override
		{
			boost::asio::post(
				ws_.get_executor(),
				boost::beast::bind_front_handler(
					&BeastConnection::doClose,
					shared_from_this()));
		}

		void setMessageHandler(
			std::function<void(ConnectionPtr,std::string)> h)
		{
			messageHandler_ = std::move(h);
		}

	private:
		void onAccept(boost::system::error_code ec)
		{
			if(ec) return;

			doRead();
		}

		void doRead()
		{
			ws_.async_read(
				buffer_,
				boost::beast::bind_front_handler(
					&BeastConnection::onRead,
					shared_from_this()));
		}

		void onRead(boost::system::error_code ec, std::size_t)
		{
			if(ec) return;

			std::string msg =
				boost::beast::buffers_to_string(buffer_.data());

			buffer_.consume(buffer_.size());

			if(messageHandler_)
				messageHandler_(shared_from_this(), std::move(msg));

			doRead();
		}

		void enqueueWrite(std::string msg)
		{
			bool writing = !writeQueue_.empty();

			writeQueue_.push_back(std::move(msg));

			if(!writing)
				doWrite();
		}

		void doWrite()
		{
			ws_.async_write(
				boost::asio::buffer(writeQueue_.front()),
				boost::beast::bind_front_handler(
					&BeastConnection::onWrite,
					shared_from_this()));
		}

		void onWrite(boost::system::error_code ec, std::size_t)
		{
			if(ec) return;

			writeQueue_.pop_front();

			if(!writeQueue_.empty())
				doWrite();
		}

		void doClose()
		{
			ws_.async_close(
				boost::beast::websocket::close_code::normal,
				[](auto){});
		}

	private:
		websocket ws_;
		boost::beast::flat_buffer buffer_;

		std::deque<std::string> writeQueue_;

		std::function<void(ConnectionPtr,std::string)> messageHandler_;
	};
}

#endif	// __BEAST_CONNECTION_HPP__
