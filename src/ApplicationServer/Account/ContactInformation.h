#ifndef ADDRESSINFORMATION_H
#define ADDRESSINFORMATION_H

#include <string>

/// @brief save contact information about an account
class ContactInformation
{
public:
	ContactInformation() = default;

    // TODO
    std::string getAddress() const;
    std::string getTelephone() const;
};

#endif // ADDRESSINFORMATION_H