//
// Created by ovv on 16.07.17.
//

#include "ConnectionHandler.h"

ConnectionHandler::ConnectionHandler(std::string root_directory)
:   request_(),
    parser_(request_),
    is_handled_(false),
    root_directory_(root_directory)
{}

ConnectionHandler::ConnectionHandler(const ConnectionHandler& handler)
:   request_(),
    parser_(request_),
    is_handled_(false),
    root_directory_(handler.root_directory_)
{}

bool ConnectionHandler::handle(char* buffer, size_t bytes_received)
{
    if (!parser_.isComplete()) {
        bool parse_success = parser_.parse(buffer, bytes_received);
        if (!parse_success)
            return false;
    }
    if (parser_.isComplete()) {
        handleRequest();
        is_handled_ = true;
    }
    return true;
}

bool ConnectionHandler::isComplete()
{
    return is_handled_;
}

void ConnectionHandler::handleRequest()
{
    // Decode url to path.
    std::string request_path;
    if (!urlDecode(request_.uri, request_path)) {
        reply_ = Reply::getDefaulReply(Reply::StatusType::bad_request);
        return;
    }

    deleteGetParams(request_path);
    //                                                 01234
    // путь должен начинатсья с /get/ т.е запросы вида /get/*
    size_t pos_get = request_path.find("/get/");
    if (pos_get != 0) {
        reply_ = Reply::getDefaulReply(Reply::StatusType::forbidden);
    }
    request_path.erase(0, 4); // удаляем /get чтобы /get/file_name -> /file_name


    // only get request
    if (request_.method == "POST") {
        reply_ = Reply::getDefaulReply(Reply::StatusType::bad_request);
        return;
    }

    // Request path must be absolute and not contain "..".
    if (request_path.empty() || request_path[0] != '/'
        || request_path.find("/..") != std::string::npos) {
        reply_ = Reply::getDefaulReply(Reply::StatusType::bad_request);
        return;
    }

    // If path ends in slash (i.e. is a directory) then add "index.html".
    if (request_path[request_path.size() - 1] == '/') {
        request_path += "index.html";
    }

    // Determine the file extension.
    std::size_t last_slash_pos = request_path.find_last_of("/");
    std::size_t last_dot_pos = request_path.find_last_of(".");
    std::string extension;
    if (last_dot_pos != std::string::npos && last_dot_pos > last_slash_pos) {
        extension = request_path.substr(last_dot_pos + 1);
    }

    // Open the file to send back.
    std::string full_path = root_directory_ + request_path;
    std::ifstream is(full_path.c_str(), std::ios::in | std::ios::binary);
    if (!is) {
        if (request_path.find("index.html") != std::string::npos) {
            reply_ = Reply::getDefaulReply(Reply::StatusType::forbidden);
        } else {
            reply_ = Reply::getDefaulReply(Reply::StatusType::not_found);
        }
        return;
    }

    // Fill out the Reply to be sent to the client.
    reply_.headers.resize(3);
    if (request_.method == "GET") {
        reply_.status = Reply::StatusType::ok;
        char buf[1024];
        while (is.read(buf, sizeof(buf)).gcount() > 0)
            reply_.content.append(buf, is.gcount());
        reply_.headers[0].name = "Content-Length";
        reply_.headers[0].value = boost::lexical_cast<std::string>(reply_.content.size());
    } else if (request_.method == "HEAD") {
        reply_.headers[0].name = "Content-Length";
        boost::filesystem::path p(full_path);
        reply_.headers[0].value = std::to_string(boost::filesystem::file_size(p));
    }


    reply_.headers[1].name  = "Content-Type";
    reply_.headers[1].value = transformExtensionToType(extension);
    reply_.headers[2].name  = "Server";
    reply_.headers[2].value = "mycpp_server";
}

std::vector<boost::asio::const_buffer> ConnectionHandler::getResponse()
{
    return reply_.toBuffers();
}

bool ConnectionHandler::urlDecode(const std::string &in, std::string &out) {
    out.clear();
    out.reserve(in.size());
    for (std::size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '%') {
            if (i + 3 <= in.size()) {
                int value = 0;
                std::istringstream is(in.substr(i + 1, 2));
                if (is >> std::hex >> value) {
                    out += static_cast<char>(value);
                    i += 2;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        } else if (in[i] == '+') {
            out += ' ';
        } else {
            out += in[i];
        }
    }
    return true;
}

void ConnectionHandler::deleteGetParams(std::string &path)
{
    std::size_t last_dot_pos = path.find_last_of(".");
    std::size_t last_question_symbol_pos = path.find_last_of("?");
    if (last_question_symbol_pos != std::string::npos && last_question_symbol_pos > last_dot_pos) {
        path.erase(last_question_symbol_pos);
    }
}