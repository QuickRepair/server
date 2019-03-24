#ifndef MERCHANTSERVERTYPE_H
#define MERCHANTSERVERTYPE_H

#include <list>
#include <string>

class MerchantServiceType
{
public:
    MerchantServiceType() = default;

    std::list<std::string> supportApplianceType() const;
    void removeSupportApplianceType(std::string type);
    void addSupportApplianceType(std::string type);

private:
	std::list<std::string> mSupportApplianceTypes;
};

#endif // MERCHANTSERVERTYPE_H
