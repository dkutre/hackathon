//
// Created by ovv on 16.07.17.
//

#include "HttpParser.h"


HttpParser::HttpParser(Request& request)
:   request_(request),
    parser_()
{
    parser_.data = &request;
    http_parser_init(&parser_, HTTP_REQUEST);
    setSettings(request);
}

bool HttpParser::isComplete()
{
    return request_.isComplete;
}

void HttpParser::setSettings(Request& request)
{
    settings_.on_message_begin = 0;  // если не занулить то внутри парсера будет SIGSEGV
    settings_.on_message_complete = 0;
    settings_.on_status = 0;
    settings_.on_body = 0;
    settings_.on_chunk_header = 0;
    settings_.on_chunk_complete = 0;


    settings_.on_header_field = [] (http_parser* parser, const char *at, size_t length) {
        Request* req = static_cast<Request*> (parser -> data);
        req -> addHeadersField(at, length);
        return 0; // нет ошибки
    };

    settings_.on_header_value = [] (http_parser* parser, const char *at, size_t length) {
        Request* req = static_cast<Request*> (parser -> data);
        req -> addHeadersValue(at, length);
        return 0; // нет ошибки
    };

    settings_.on_url = [] (http_parser* parser, const char *at, size_t length) {
        Request* req = static_cast<Request*> (parser -> data);

        if (parser -> method == http_method::HTTP_GET) {
            req -> method = "GET";
        } else if (parser -> method == http_method::HTTP_POST) {
            req -> method = "POST";
        } else if (parser -> method == http_method::HTTP_HEAD) {
            req -> method = "HEAD";
        }
        req -> setUri(at, length);
        return 0; // нет ошибки
    };

    settings_.on_headers_complete = [] (http_parser* parser) {
        Request* req = static_cast<Request*> (parser -> data);
        req -> isComplete = true;
        return 1; // прерываем парсинг, т.к мы работаем только с get запросами
    };
}

bool HttpParser::parse(char* buffer, size_t length)
{
    size_t parsed_bytes = http_parser_execute(&parser_, &settings_, buffer, length);
    return (parsed_bytes == length);
}