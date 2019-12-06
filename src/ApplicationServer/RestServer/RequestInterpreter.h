//
// Created by gaojian on 19-3-19.
//

#ifndef HARSERVER_REQUESTINTERPRETER_H
#define HARSERVER_REQUESTINTERPRETER_H

#include <map>
#include <cpprest/details/basic_types.h>
#include <cpprest/json.h>
#include <cpprest/http_headers.h>

class RespondGenerator;
class Respond;
enum class HTTPRequest;

class RequestInterpreter {
public:
	RequestInterpreter();

	Respond analysis(HTTPRequest request, std::vector<utility::string_t> uri, web::http::http_headers headers, web::json::object data);

private:
	Respond requestVerification(std::vector<utility::string_t> uri);
	Respond getToken(std::vector<utility::string_t> uri, web::json::object json);
	Respond getMerchantsList(std::vector<utility::string_t> uri);
	Respond publishOrder(web::json::object json);
	Respond getAllowedService();
	Respond getMerchantService(std::vector<utility::string_t> uri);
	Respond updateMerchantService(std::vector<utility::string_t> uri, web::http::http_headers headers, web::json::object json);
	Respond getOrderList(std::vector<utility::string_t> uri, web::http::http_headers headers);
	Respond getOrderDetail(std::vector<utility::string_t> uri, web::http::http_headers headers);
	Respond updateOrder(std::vector<utility::string_t> uri, web::http::http_headers headers, web::json::object json);
	Respond receiveOrder(std::vector<utility::string_t> uri, web::http::http_headers headers, web::json::object json);
	Respond startRepair(std::vector<utility::string_t> uri, web::http::http_headers headers, web::json::object json);
	Respond endRepair(std::vector<utility::string_t> uri, web::http::http_headers headers, web::json::object json);
	Respond pay(std::vector<utility::string_t> uri, web::http::http_headers headers, web::json::object json);
	Respond rejectOrder(std::vector<utility::string_t> uri, web::http::http_headers headers, web::json::object json);

	unsigned long toUnsignedLong(const std::string &s);
	double toDouble(const std::string &s);

	std::unique_ptr<RespondGenerator> respondGenerator;
};

#endif //HARSERVER_REQUESTINTERPRETER_H
