#include <boost/asio.hpp>

#include <array>
#include <iostream>
#include <string_view>

namespace net = boost::asio;
using tcp = net::ip::tcp;

void RunClient(std::string_view host, short port) {
    try {
        net::io_context ioc;
        tcp::socket tcp_socket(ioc);
        tcp::resolver resolver(ioc);
        net::connect(tcp_socket, resolver.resolve({host.data(), std::to_string(port)}));

        while (true) {
            std::cout << "number [1-99]: ";

            int number;
            std::cin >> number;
            if (std::cin.fail() || number < 1 || number > 99) {
                break;
            }

            auto request = std::to_string(number);
            tcp_socket.write_some(net::buffer(request, request.length()));

            std::array<char, 1024> reply;
            auto reply_length = tcp_socket.read_some(net::buffer(reply, reply.size()));

            std::cout << "reply is: ";
            std::cout.write(reply.data(), reply_length);
            std::cout << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "exception: " << e.what() << std::endl;
    }
}

int main() {
    RunClient("localhost", 11234);
}
