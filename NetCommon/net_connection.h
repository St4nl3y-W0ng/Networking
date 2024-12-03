#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"

namespace nw 
{
	namespace net 
	{
		template <typename T>
		class connection : public std::enable_shared_from_this<connection<T>>
		{
		public:
			connection()
			{}
			virtual ~connection()
			{}

		public:
			bool ConnectToServer();
			bool Disconnect();
			bool IsConnected() const;

		public:
			bool Send(const message<T>& msg);

		protected:
			// Each connection has unique socket to a remote
			asio::ip::tcp::socket m_socket;

			// This context is shared  with the whole asio instance
			asio::io_context& m_asioContext;

			// Queue sends all of the messages sent to be sent to remote side of this connection
			tsqueue<message<T>> m_qMessagesOut;

			// Reference to a Queue holds messages received from remote side of connection as owner supposed to provide connection
			tsqueue<owned_message<T>>& m_qMessagesIn;
		};
	}
}