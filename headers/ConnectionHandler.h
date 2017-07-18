//
// Created by ovv on 16.07.17.
//

#ifndef HACKATHON_CONNECTIONHANDLER_H
#define HACKATHON_CONNECTIONHANDLER_H

#include "HttpParser.h"
#include "Request.h"
#include "Reply.h"
#include <string>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include "boost/filesystem.hpp"
#include "mimeTypes.h"
#include <sstream>

class ConnectionHandler
{
public:
    ConnectionHandler(std::string root_directory);
    ConnectionHandler(const ConnectionHandler& handler);

    bool handle(char* buffer, size_t bytes_received);
    bool isComplete();
    std::vector<boost::asio::const_buffer> getResponse();

private:
    void handleRequest();
    void deleteGetParams(std::string& path);
    /// Perform URL-decoding on a string. Returns false if the encoding was
    /// invalid.
    static bool urlDecode(const std::string& in, std::string& out);

    Request request_;
    HttpParser parser_;
    Reply reply_;
    std::string root_directory_;
    bool is_handled_;
};


#endif //HACKATHON_CONNECTIONHANDLER_H
