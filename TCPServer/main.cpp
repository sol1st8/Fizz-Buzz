#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/asio.hpp>

#include <array>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>

namespace net = boost::asio;
using tcp = net::ip::tcp;

std::string FizzBuzz(int number) {
    if (number != 0) {
        auto m3 = number % 3;
        auto m5 = number % 5;
        if (m3 == 0 && m5 == 0) {
            return "fizzbuzz";
        }
        else if (m5 == 0) {
            return "buzz";
        }
        else if (m3 == 0) {
            return "fizz";
        }
    }

    return std::to_string(number);
}

class Session : public std::enable_shared_from_this<Session> {
  public:
    explicit Session(tcp::socket socket) : tcp_socket_(std::move(socket)) {}

    void Start() {
        Read();
    }

  private:
    std::array<char, 1024> data;
    tcp::socket tcp_socket_;

    void Read() {
        tcp_socket_.async_read_some(net::buffer(data, data.size()), [this, self = shared_from_this()](std::error_code ec, std::size_t length) {
            if (!ec) {
                auto number = std::string(data.data(), length);
                auto result = FizzBuzz(std::atoi(number.c_str()));

                std::cout << number << " -> " << result << std::endl;

                Write(result);
            }
        });
    }

    void Write(std::string_view response) {
        tcp_socket_.async_write_some(net::buffer(response.data(), response.size()), [this, self = shared_from_this()](std::error_code ec, std::size_t) {
            if (!ec) {
                Read();
            }
        });
    }
};

class Server {
  public:
    Server(net::io_context& context, short port) : tcp_acceptor_(context, tcp::endpoint(tcp::v4(), port)), tcp_socket_(context) {
        std::cout << "Server running on port " << port << std::endl;

        Accept();
    }

  private:
    tcp::acceptor tcp_acceptor_;
    tcp::socket tcp_socket_;

    void Accept() {
        tcp_acceptor_.async_accept(tcp_socket_, [this](std::error_code ec) {
            if (!ec) {
                std::make_shared<Session>(std::move(tcp_socket_))->Start();
            }

            Accept();
        });
    }
};

void RunServer(short port) {
    try {
        net::io_context ioc;

        Server srv(ioc, port);

        ioc.run();
    }
    catch (const std::exception& e) {
        std::cerr << "exception: " << e.what() << std::endl;
    }
}

int main() {
    RunServer(11234);
}
