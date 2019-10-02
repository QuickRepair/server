//
// Created by gaojian on 19-3-15.
//

#ifndef HARSERVER_RESTHANDLER_H
#define HARSERVER_RESTHANDLER_H

#include <cpprest/http_listener.h>

class RequestInterpreter;

enum class HTTPRequest {
	GET, PUT, POST, DELETE
};

class RestHandler {
public:
    explicit RestHandler(std::string url);
    ~RestHandler();

private:
	/// @brief Handle http operates
    void handleGet(web::http::http_request msg);
    void handlePut(web::http::http_request msg);
    void handlePost(web::http::http_request msg);
    void handleDelete(web::http::http_request msg);

    web::http::experimental::listener::http_listener m_listener;

    std::unique_ptr<RequestInterpreter> m_analyser;
};

class Respond {
public:
	Respond() = default;
	Respond(web::http::status_code statusCode);
	Respond(web::json::value &&json);
	Respond(web::http::status_code statusCode, web::json::value &&json);

	const web::json::value &json() const;
	web::http::status_code statusCode() const;

private:
	web::json::value m_json;
	web::http::status_code m_statusCode;
};

#endif //HARSERVER_RESTHANDLER_H
