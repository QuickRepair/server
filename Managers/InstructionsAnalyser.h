//
// Created by gaojian on 19-3-19.
//

#ifndef HARSERVER_INSTRUCTIONSANALYSER_H
#define HARSERVER_INSTRUCTIONSANALYSER_H

#include <map>
#include <cpprest/details/basic_types.h>
#include <cpprest/json.h>

class InstructionsAnalyser {
public:
	std::string instructionFromMap(std::map<utility::string_t, utility::string_t> instruction);
	std::string instructionFromJson(web::json::value json);

private:
	std::string doGetVerification(std::map<utility::string_t, utility::string_t> &instruction);
	std::string doLogin(web::json::object &object);
	std::string doGetList(std::map<utility::string_t, utility::string_t> &instruction);
	std::string doUpdateServiceType(web::json::object &object);
	std::string doSubmitOrder(web::json::object &object);
};


#endif //HARSERVER_INSTRUCTIONSANALYSER_H
