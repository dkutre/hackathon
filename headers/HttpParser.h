//
// Created by ovv on 16.07.17.
//

#ifndef HACKATHON_HTTPPARSER_H
#define HACKATHON_HTTPPARSER_H

#include "http_parser.h"
#include "Request.h"


class HttpParser
{
public:
    HttpParser(Request& request);

    bool parse(char* buffer, size_t length);
    bool isComplete();

private:
    void setSettings(Request& request);

    http_parser parser_;
    Request& request_;
    http_parser_settings settings_;

};


#endif //HACKATHON_HTTPPARSER_H
