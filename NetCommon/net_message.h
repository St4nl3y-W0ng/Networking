#pragma once
#include "net_common.h"

namespace nw {
	namespace net {

		/*Message Header is sent at the start of all messages. This template allows us 
		to use the enum class to ensure that all messages are valid at compile time.*/

		template <typename T>
		struct message_header 
		{
			T id{};
			uint32_t size = 0;
		};

		template <typename T>
		struct message 
		{
			message_header<T> header{};
			std::vector<uint8_t> body;
			// returns size of entire message packet in bytes
			size_t size() const 
			{
				return sizeof(message_header<T>) + body.size();
			}

			// Override the std::cout compatability - produces friendly description of messages
			friend std::ostream& operator << (std::ostream& os, const message<T>& msg) 
			{
				os << "ID: " << int(msg.header.id) << "Size: " << msg.header.size;
				return os;
			}

			// Pushes any POD-like data into the message buffer
			template<typename DataType>
			friend message<T>& operator << (message<T>& msg, const DataType& data) 
			{
				// Check that the type of the data being pushed can be trivially copied
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into the vector");

				// Cache the current size of body vector, as this is the point where data is inserted
				size_t i = msg.body.size();

				// Resize the vector by the size of the data being pushed in
				msg.body.resize(i + sizeof(DataType));

				// Physically copy the data into the newly allocated vector space
				std::memcpy(msg.body.data() + i, &data, sizeof(DataType));

				// Recalculate the message size
				msg.header.size = msg.size();

				// Return the target message so it can be chained
				return msg;
			}

			template<typename DataType>
			friend message<T>& operator >> (message<T>& msg, DataType& data) 
			{
				// Check that the type of the data being pushed can be trivially copied
				static_assert(std::is_standard_layout<DataType>::value, "Data is too complex to be pushed into the vector");

				// Cache the location towards the end of the vector where the pulled data starts
				size_t i = msg.body.size() - sizeof(DataType);

				// Physically copy the data from the vector to the user	variable
				std::memcpy(&data, msg.body.data() + i, sizeof(DataType));

				// Shrink the vector to remove read bytes, and reset end position
				msg.body.resize(i);

				// Recalculate the message size
				msg.header.size = msg.size();

				// Return the target message so it can be chained
				return msg;
			}
		};

		template <typename T>
		class connection;

		template <typename T>
		struct owned_message 
		{
			std::shared_ptr<connection<T>> remote = nullptr;
			message<T> msg;

			// friendly neighbourhood string maker
			friend std::ostream& operator << (std::ostream& os, const owned_message<T>& msg) 
			{
				os << msg.msg;
				return os;
			}
		};
		
	}
}