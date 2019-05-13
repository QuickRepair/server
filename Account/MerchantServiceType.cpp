#include "MerchantServiceType.h"
#include <algorithm>

using std::find_if;					using std::string;

MerchantServiceType::MerchantServiceType(std::list<std::string> supportList, double maxDistance)
	: mSupportApplianceTypes{std::move(supportList)}, mMaxRepairDistance{maxDistance}
{}

std::list<std::string> MerchantServiceType::supportApplianceType() const
{
	return mSupportApplianceTypes;
}

void MerchantServiceType::setSupportApplianceType(std::list<std::string> types)
{
	mSupportApplianceTypes = std::move(types);
}

double MerchantServiceType::maxRepairDistance() const
{
	return mMaxRepairDistance;
}

void MerchantServiceType::setMaxRepairDistance(double distance)
{
	mMaxRepairDistance = distance;
}