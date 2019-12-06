//
// Created by gaojian on 19-3-15.
//

#include "RestHandler.h"
#include "src/ApplicationServer/ManagerProxy/AccountManagerProxy.h"
#include "RequestInterpreter.h"
#include "RespondGenerator.h"

using web::http::methods;					using std::bind;
using web::uri;								using std::map;
using std::make_unique;						using web::http::status_codes;
using std::string;							using utility::conversions::to_utf8string;
using utility::conversions::to_string_t;

RestHandler::RestHandler(std::string url)
    : m_listener{to_string_t(url)}
{
	m_analyser = make_unique<RequestInterpreter>();
	
	m_listener.support(methods::GET, bind(&RestHandler::handleGet, this, std::placeholders::_1));
	m_listener.support(methods::POST, bind(&RestHandler::handlePost, this, std::placeholders::_1));
	m_listener.support(methods::PUT, bind(&RestHandler::handlePut, this, std::placeholders::_1));
	m_listener.support(methods::DEL, bind(&RestHandler::handleDelete, this, std::placeholders::_1));

	m_listener.open().wait();
}

RestHandler::~RestHandler()
{
	m_listener.close().wait();
}


void RestHandler::handleGet(web::http::http_request msg)
{
	//std::cout << msg.to_string() << std::endl;
	// find get's parameters
	std::vector<utility::string_t> uri = uri::split_path(msg.request_uri().path());
	web::http::http_headers headers = msg.headers();
	web::json::object data = msg.extract_json().get().as_object();

//	std::cout << "get:" << std::endl;
//	for(auto &s : uri)
//		std::cout << to_utf8string(s) << std::endl;

	Respond replyMsg = m_analyser->analysis(HTTPRequest::GET, uri, headers, data);
//	std::cout << to_utf8string(replyMsg.json().as_string()) << std::endl;
	msg.reply(replyMsg.statusCode(), replyMsg.json());
}

void RestHandler::handlePut(web::http::http_request msg)
{
	std::cout << to_utf8string(msg.to_string()) << std::endl;
}

void RestHandler::handlePost(web::http::http_request msg)
{
	//std::cout << msg.to_string() << std::endl;
	std::vector<utility::string_t> uri = uri::split_path(msg.request_uri().path());
	web::http::http_headers headers = msg.headers();
	web::json::object data = msg.extract_json().get().as_object();

//	std::cout << "post:" << std::endl;
//	std::cout << to_utf8string(json()) << std::endl;

	Respond replyMsg = m_analyser->analysis(HTTPRequest::POST, uri, headers, data);
	msg.reply(replyMsg.statusCode(), replyMsg.json());
}

void RestHandler::handleDelete(web::http::http_request msg)
{
	std::cout <<to_utf8string(msg.to_string()) << std::endl;
}

Respond::Respond(web::http::status_code statusCode)
	: m_statusCode{web::http::status_codes::OK}
{}

Respond::Respond(web::json::value &&json)
	: m_statusCode{web::http::status_codes::OK}, m_json{json}
{}

Respond::Respond(web::http::status_code statusCode, web::json::value &&json)
	: m_statusCode{statusCode}, m_json{json}
{}

const web::json::value& Respond::json() const
{
	return m_json;
}

web::http::status_code Respond::statusCode() const
{
	return m_statusCode;
}
