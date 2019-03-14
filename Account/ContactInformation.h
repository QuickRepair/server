#ifndef ADDRESSINFORMATION_H
#define ADDRESSINFORMATION_H

#include <string>
#include <vector>

class ContactInformation
{
public:
    ContactInformation();

private:
	std::string m_address;
	std::vector<std::string> m_tel;
};

#endif // ADDRESSINFORMATION_H