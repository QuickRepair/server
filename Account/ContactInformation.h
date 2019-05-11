#ifndef ADDRESSINFORMATION_H
#define ADDRESSINFORMATION_H

#include <string>

/// @brief save contact information about an account
class ContactInformation
{
public:
	ContactInformation() = default;
    explicit ContactInformation(std::string address, std::string telephone);

private:
	std::string m_address;
	std::string m_tel;
};

#endif // ADDRESSINFORMATION_H