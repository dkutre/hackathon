//
// Created by ovv on 13.05.17.
//

#ifndef HIGHLOAD_SERVER_SERVER_H
#define HIGHLOAD_SERVER_SERVER_H

#include <boost/asio.hpp>
#include <thread>
#include "Connection.h"

class Server
{
public:
    Server(const Server&)             = delete;
    Server& operator= (const Server&) = delete;

    Server(const std::string& address, const std::string& port,
           const std::string& root_directory, size_t number_threads);
    void run();

private:
    void startAccept();
    void doAwaitStop();

    std::size_t thread_pool_size_;
    boost::asio::io_service service_;
    boost::asio::signal_set signals_;
    boost::asio::ip::tcp::acceptor acceptor_;
    connectionPointer new_connection_;
    ConnectionHandler handler_;
};


#endif //HIGHLOAD_SERVER_SERVER_H
