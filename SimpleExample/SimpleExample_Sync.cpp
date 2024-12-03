#include <iostream>

#define ASIO_STANDALONE
#define _WIN32_WINNT 0x0A00
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

int main()
{
	asio::error_code ec;
	
	// Create a context - platform specific interface
	asio::io_context context;

	// Get the address of connection destination
	asio::ip::tcp::endpoint endpoint(asio::ip::make_address("172.253.118.104", ec), 80); //127.0.0.1

	// Create socket, context will deliver implementation
	asio::ip::tcp::socket socket(context);
	
	// Try connecting
	socket.connect(endpoint, ec);

	if (!ec) {
		std::cout << "Connected!" << std::endl;
	}
	else {
		std::cout << "Failed to connect to address:\n" << ec.message() << std::endl;
	}

	if (socket.is_open()) {
		std::string sRequest =
			"GET /index.html HTTP/1.1\r\n"
			"Host: example.com\r\n"
			"Connection: close\r\n\r\n";
		socket.write_some(asio::buffer(sRequest.data(), sRequest.size()), ec);

		// using namespace std::chrono_literals;
		// std::this_thread::sleep_for(200ms);

		socket.wait(socket.wait_read);

		size_t bytes = socket.available();
		std::cout << "Bytes available: " << bytes << std::endl;

		if (bytes > 0) {
			std::vector<char> vBuffer(bytes);
			socket.read_some(asio::buffer(vBuffer.data(), vBuffer.size()), ec);

			for (auto c : vBuffer) {
				std::cout << c;
			}
		}
	}


	system("pause");

	return 0;
}