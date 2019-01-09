#ifndef SERVER_H
#define SERVER_H

#include <memory>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

using namespace std;
using namespace boost::asio;

class Session
{
private:
  ip::tcp::socket socket_;
  static const long max_length = 1024*1024;
  char data_[max_length]; // message

  void handle_read(const boost::system::error_code& error,
      size_t bytes_transferred);
public:
  Session(boost::asio::io_service& io_service)
    : socket_(io_service) {}

  ip::tcp::socket& socket() { return socket_; }
  void start();
};

class Server
{
private:
    io_service& io_service_;
    ip::tcp::acceptor acceptor_;

    void start_accept();
    void handle_accept(Session* new_session, const boost::system::error_code& error);
public:
    Server(io_service& io_service, string ip, short port);
};

#endif // SERVER_H
