//
// Created by ovv on 13.05.17.
//

#ifndef HIGHLOAD_SERVER_REQUEST_H
#define HIGHLOAD_SERVER_REQUEST_H

#include <string>
#include "Header.h"
#include <vector>

struct Request
{
    Request();

    std::string method;
    std::string uri;
    int http_version_major;
    int http_version_minor;
    std::vector<Header> headers;
    bool isComplete;

    void setUri(const char* str, size_t length);
    void addHeadersField(const char* str, size_t length);
    void addHeadersValue(const char* str, size_t length);
};


#endif //HIGHLOAD_SERVER_REQUEST_H
