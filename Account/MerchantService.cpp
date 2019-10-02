#include "MerchantService.h"
#include "Account/MerchantAccount.h"
#include "DataSource/DataSource.hpp"

using std::string;

MerchantService::MerchantService(MerchantAccount *merchant)
	: _merchant{merchant}
{}

std::list<std::string> MerchantService::supportApplianceType() const
{
	return DataSource::getDataAccessInstance()->getMerchantSupportedApplianceTypes(_merchant->id());
}

void MerchantService::setSupportApplianceType(std::list<std::string> types)
{
	DataSource::getDataAccessInstance()->updateSupportedAppliances(_merchant->id(), types);
}

double MerchantService::maxRepairDistance() const
{
	return DataSource::getDataAccessInstance()->getMaxSupportDistance(_merchant->id());
}

void MerchantService::setMaxRepairDistance(double distance)
{
	DataSource::getDataAccessInstance()->setMaxSupportDistance(_merchant->id(), distance);
}

std::list<std::string> MerchantService::getAllowedService()
{
	return DataSource::getDataAccessInstance()->getAllowedService();
}

void MerchantService::updateService(std::list<std::string> appliances, double distance)
{
	DataSource::getDataAccessInstance()->updateMerchantServices(_merchant->id(), appliances, distance);
}
