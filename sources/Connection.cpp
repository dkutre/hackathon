//
// Created by ovv on 13.05.17.
//

#include "Connection.h"

Connection::Connection(boost::asio::io_service& service, ConnectionHandler handler):
   strand_(service),
   socket_(service),
   handler_(handler)
{}

boost::asio::ip::tcp::socket& Connection::getSocket()
{
    return socket_;
}

void Connection::start()
{
    auto shared_this = shared_from_this();
    socket_.async_read_some(boost::asio::buffer(buffer_),
                            strand_.wrap([shared_this](const boost::system::error_code& e,
                                                       std::size_t bytes_transferred) {
                                shared_this->handleRead(e, bytes_transferred);
                            }));
}

void Connection::handleRead(const boost::system::error_code &e, std::size_t bytes_transferred)
{
    if (!e) {
        auto shared_this = shared_from_this();
        bool success = handler_.handle(buffer_.data(), bytes_transferred);

        if (!success) {
            // parse error
            socket_.async_write_some(Reply::getDefaulReply(Reply::StatusType::bad_gateway).toBuffers(),
                                     strand_.wrap([shared_this](const boost::system::error_code &e, size_t bytes) {
                                         shared_this->handleWrite(e);
                                     }));
            return;
        }

        if (handler_.isComplete()) {
            // write
            socket_.async_write_some(handler_.getResponse(),
                                     strand_.wrap([shared_this](const boost::system::error_code &e, size_t bytes) {
                                         shared_this->handleWrite(e);
                                     }));
        } else {
            // read
            socket_.async_read_some(boost::asio::buffer(buffer_),
                                    strand_.wrap([shared_this](const boost::system::error_code &e, size_t bytes) {
                                        shared_this->handleRead(e, bytes);
                                    }));
        }
    }
}

void Connection::handleWrite(const boost::system::error_code &e)
{
    if (!e) {
        // Initiate graceful connection closure.
        boost::system::error_code ignored_ec;
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
    }
}