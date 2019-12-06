#ifndef MERCHANTSERVERTYPE_H
#define MERCHANTSERVERTYPE_H

#include <list>
#include <string>

class MerchantAccount;

/// @brief The class represent the merchant supported service types
class MerchantService
{
public:
//	MerchantServiceType() : mMaxRepairDistance{0} {};
    MerchantService(MerchantAccount *merchant);

	void updateService(std::list<std::string> appliances, double distance);
	/// @brief Setters and getters
    std::list<std::string> supportApplianceType() const;
    void setSupportApplianceType(std::list<std::string> types);

	double maxRepairDistance() const;

    void setMaxRepairDistance(double distance);
	static std::list<std::string> getAllowedService();

private:
	MerchantAccount *_merchant;
//	std::list<std::string> mSupportApplianceTypes;
//	double mMaxRepairDistance;
};

#endif // MERCHANTSERVERTYPE_H
