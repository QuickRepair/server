#ifndef ADDRESSINFORMATION_H
#define ADDRESSINFORMATION_H

#include <string>
#include <vector>

/// @brief save all contact information about an account
class ContactInformation
{
public:
	ContactInformation() = default;
    explicit ContactInformation(std::string address);

private:
	std::string m_address;
	std::vector<std::string> m_tel;
};

#endif // ADDRESSINFORMATION_H