//
// Created by ovv on 16.07.17.
//

#include "Request.h"

Request::Request()
{
    isComplete = false;
}

void Request::setUri(const char *str, size_t length)
{
    uri.append(str, length);
}

void Request::addHeadersField(const char *str, size_t length)
{
    headers.push_back(Header());
    headers.back().name.append(str, length);
}

void Request::addHeadersValue(const char *str, size_t length)
{
    headers.back().value.append(str, length);
}