#include "server.h"
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <streambuf>
#include <cstring>

Server::Server(io_service& io_service, string ip, short port)
  : io_service_(io_service),
    acceptor_(io_service, ip::tcp::endpoint(ip::address_v4::from_string(ip), port))
{
    start_accept();
}

void Server::start_accept()
{
    cout << "listening...\n";
    Session* new_session = new Session(io_service_);
    acceptor_.async_accept(new_session->socket(),
                boost::bind(&Server::handle_accept, this, new_session,
                           boost::asio::placeholders::error));
}

void Server::handle_accept(Session* new_session,
    const boost::system::error_code& error)
{
    if (!error)
    {
        cout << "new session\n";
        new_session->start();
    }
    else
    {
        cout << "error session\n";
        delete new_session;
    }
    start_accept();
}

/////////////////////////////////////////////////////////////////////////////////////

void Session::start()
{
    cout << "session start\n";
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
            boost::bind(&Session::handle_read, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
}

void Session::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{
#pragma omp single
{
    if (!error)
    {
        string readingData(data_);
        vector<string> requestSplit;
        boost::algorithm::split(requestSplit, readingData, boost::is_any_of(" "));
        for (auto& x: requestSplit)
            boost::trim(x);
        if (requestSplit.front() == "GET")
        {
            cout << "finded get file = " << requestSplit.at(1) << endl;
            requestSplit.at(1).erase(0, 1); // erase slash in front
            if (find(requestSplit.at(1).begin(), requestSplit.at(1).end(), '?') != requestSplit.at(1).end())
            {
                cout << "find params\n";
                vector<string> getSplit;
                boost::algorithm::split(getSplit, requestSplit.at(1), boost::is_any_of("?"));
                requestSplit.at(1) = getSplit.front();
            }
            cout << "open file " << requestSplit.at(1) << endl;
            ifstream f(requestSplit.at(1));
            if (f.is_open())
            {
                cout << "send file...\n";
                std::string fileToSend;
                f.seekg(0, std::ios::end);
                fileToSend.reserve(f.tellg());
                f.seekg(0, std::ios::beg);
                fileToSend.assign((istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
                fileToSend = "HTTP/1.0 200 OKr\r\n\r\n" + fileToSend;
                boost::asio::write(socket_, boost::asio::buffer(fileToSend.data(), fileToSend.size()));
                cout << "delete session\n";
                delete this;
            }
            else
            {
                cout << "cant open file\nsend 404\n";
                char error404[] = "HTTP/1.0 404 NOT FOUND\r\n";
                boost::asio::write(socket_, boost::asio::buffer(error404, 25));
                cout << "delete session\n";
                delete this;
            }
        }
    }
}
}
