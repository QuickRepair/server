#ifndef MERCHANTSERVERTYPE_H
#define MERCHANTSERVERTYPE_H

#include <list>
#include <string>

class MerchantServiceType
{
public:
    MerchantServiceType(std::list<std::string> supportList, int maxDistance);

    std::list<std::string> supportApplianceType() const;
    void setSupportApplianceType(std::list<std::string> types);

    int maxRepairDistance() const;
    void setMaxRepairDistance(int distance);

private:
	std::list<std::string> mSupportApplianceTypes;
	int mMaxRepairDistance;
};

#endif // MERCHANTSERVERTYPE_H
