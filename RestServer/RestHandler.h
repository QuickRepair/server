//
// Created by gaojian on 19-3-15.
//

#ifndef HARSERVER_RESTHANDLER_H
#define HARSERVER_RESTHANDLER_H

#include <cpprest/http_listener.h>
#include "../Managers/InstructionsAnalyser.h"

class RestHandler {
public:
    explicit RestHandler(utility::string_t url);
    ~RestHandler();

private:
    void handleGet(web::http::http_request msg);
    void handlePut(web::http::http_request msg);
    void handlePost(web::http::http_request msg);
    void handleDelete(web::http::http_request msg);

    web::http::experimental::listener::http_listener m_listener;

    std::shared_ptr<InstructionsAnalyser> m_analyser;
};

#endif //HARSERVER_RESTHANDLER_H
