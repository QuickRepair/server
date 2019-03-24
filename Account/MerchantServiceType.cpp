#include "MerchantServiceType.h"
#include <algorithm>

using std::find_if;					using std::string;

std::list<std::string> MerchantServiceType::supportApplianceType() const
{
	return mSupportApplianceTypes;
}

void MerchantServiceType::removeSupportApplianceType(std::string type)
{
	mSupportApplianceTypes.remove(type);
}

void MerchantServiceType::addSupportApplianceType(std::string type)
{
	mSupportApplianceTypes.push_back(type);
}