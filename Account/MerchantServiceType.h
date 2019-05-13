#ifndef MERCHANTSERVERTYPE_H
#define MERCHANTSERVERTYPE_H

#include <list>
#include <string>

/// @brief The class represent the merchant supported service types
class MerchantServiceType
{
public:
	MerchantServiceType() : mMaxRepairDistance{0} {};
    MerchantServiceType(std::list<std::string> supportList, double maxDistance);

    /// @brief Setters and getters
    std::list<std::string> supportApplianceType() const;
    void setSupportApplianceType(std::list<std::string> types);

    double maxRepairDistance() const;
    void setMaxRepairDistance(double distance);

private:
	std::list<std::string> mSupportApplianceTypes;
	double mMaxRepairDistance;
};

#endif // MERCHANTSERVERTYPE_H
