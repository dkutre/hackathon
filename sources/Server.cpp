//
// Created by ovv on 13.05.17.
//

#include "Server.h"
#include <iostream>

Server::Server(const std::string& address, const std::string& port, const std::string& root_directory,
               size_t number_threads):
    service_(),
    acceptor_(service_),
    signals_(service_),
    thread_pool_size_(number_threads),
    handler_(root_directory)
{
    signals_.add(SIGINT);
    signals_.add(SIGTERM);

    doAwaitStop();

    boost::asio::ip::tcp::resolver resolver(service_);
    boost::asio::ip::tcp::resolver::query query(address, port);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(query);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    startAccept();
}

void Server::run()
{
    std::vector<std::shared_ptr<std::thread>> threads;
    for (std::size_t i = 0; i < thread_pool_size_; ++i) {
        std::shared_ptr<std::thread> thread(new std::thread(
                [this] () {
                    service_.run();
                }));
        threads.push_back(thread);
    }

    for (std::size_t i = 0; i < threads.size(); ++i)
        threads[i]->join();
    std::cout << "finished";
}


void Server::doAwaitStop()
{
    signals_.async_wait([this] (const boost::system::error_code& e, int signal_no) {
        service_.stop();
    });
}

void Server::startAccept()
{
    new_connection_.reset(new Connection(service_, handler_));
    acceptor_.async_accept(new_connection_ -> getSocket(),
                           [this] (const boost::system::error_code& e) {
                               if (!e) {
                                   new_connection_->start();
                               }
                               startAccept();
                           });
}