//
// Created by gaojian on 19-3-15.
//

#include "RestHandler.h"
#include "../Managers/AccountManager.h"

using web::http::methods;			using std::bind;
using web::uri;						using std::map;
using std::make_shared;				using web::http::status_codes;
using std::string;

RestHandler::RestHandler(utility::string_t url)
    : m_listener{url}
{
	m_analyser = make_shared<InstructionsAnalyser>();
	
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
	std::map<utility::string_t, utility::string_t> query = uri::split_query(msg.relative_uri().query());
	std::cout << "get:" << std::endl;
	for(auto &s : query)
		std::cout << s.first << " " << s.second << std::endl;
	
	string replyMsg = m_analyser->instructionFromMap(query);
	std::cout << replyMsg << std::endl;
	msg.reply(status_codes::OK, replyMsg);
}

void RestHandler::handlePut(web::http::http_request msg)
{
	//TODO
	std::cout << msg.to_string() << std::endl;
}

void RestHandler::handlePost(web::http::http_request msg)
{
	//std::cout << msg.to_string() << std::endl;
	pplx::task<web::json::value> aTask = msg.extract_json();
	web::json::value json = aTask.get();

	std::cout << "post:" << std::endl;
	std::cout << json.serialize() << std::endl;

	string replyMsg = m_analyser->instructionFromJson(json);
	msg.reply(status_codes::OK, replyMsg);
}

void RestHandler::handleDelete(web::http::http_request msg)
{
	//TODO
	std::cout << msg.to_string() << std::endl;
}
