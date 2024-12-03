#pragma once

#include "net_common.h"
#include "net_tsqueue.h"
#include "net_message.h"

namespace nw
{
	namespace net
	{
		template <typename T>
		class client_interface
		{
		public:
			client_interface(): m_socket(m_context)
			{
				// initialise socket with io context to perform tasks
			}
			virtual ~client_interface()
			{
				// Client destroyed, should disconnect from server
				Disconnect();
			}

		public:
			// Connect to server with hostname/ip address and port
			bool Connect(const std::string& host, const uint16_t port) 
			{
				try
				{
					// Create connection
					m_connection = std::make_unique<connection<T>>(); //TODO

					// Resolve hostname/ip address into tangible physical address
					asio::ip::tcp::resolver resolver(m_context);
					m_endpoints = resolver.resolve(host, std::to_string(port));

					// Tell connection to connect to server
					m_connection->ConnectToServer(m_endpoints);

					// Start context thread
					thrContext = std::thread([this]() {m_context.run();});
				} 
				catch (std::exception& e)
				{
					std::cerr << "Client exception:" << e.what() << "\n";
					return false;
				}
				return true;
			};

			// Disconnect from server
			void Disconnect()
			{
				// check if connect, disconnect if so
				if (IsConnected())
				{
					m_connection->Disconnect();
				}

				// Stop context and join thread
				m_context.stop();
				
				if (thrContext.joinable()) {
					thrContext.join();
				}

				// Destroy connection obj
				m_connection.release();
			};
			// Check if client is connected to server
			bool IsConnected() 
			{
				if (m_connection) 
					return m_connection->IsConnected();	
				else
					return false;
			};

			// Retrieves queue of messages in
			tsqueue<owned_message<T>>& Incoming()
			{
				return m_qMessagesIn;
			}

		protected:
			// asio context for data transfer
			asio::io_context m_context;
			// requires own thread
			std::thread thrContext;
			// hardware socket connected to server
			asio::ip::tcp::socket m_socket;
			// client has single instance of connection object, which does data transfer
			std::unique_ptr<connection<T>> m_connection;

		private:
			// Threadsafe queue for incoming messages from server
			tsqueue<owned_message> m_qMessagesIn;

			// Reference to a Queue holds messages received from remote side of connection as owner supposed to provide connection
			tsqueue<message<T>> m_qMessagesOut;
		};
	}
}