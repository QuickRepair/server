//
// Created by gaojian on 19-3-19.
//

#ifndef HARSERVER_INSTRUCTIONSANALYSER_H
#define HARSERVER_INSTRUCTIONSANALYSER_H

#include <map>
#include <cpprest/details/basic_types.h>

class InstructionsAnalyser {
public:
	void instructionFromMap(std::map<utility::string_t, utility::string_t> instruction);
	void instructionFromJson();
};


#endif //HARSERVER_INSTRUCTIONSANALYSER_H
