#include "Thread/ThreadPool.hpp"
#include "MerchantService.h"
#include "Account.h"
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
	ThreadPool::getInstance().submit(&DataAccess::updateSupportedAppliances, DataSource::getDataAccessInstance(), _merchant->id(), types);
}

double MerchantService::maxRepairDistance() const
{
	return DataSource::getDataAccessInstance()->getMaxSupportDistance(_merchant->id());
}

void MerchantService::setMaxRepairDistance(double distance)
{
	ThreadPool::getInstance().submit(&DataAccess::setMaxSupportDistance, DataSource::getDataAccessInstance(), _merchant->id(), distance);
}

std::list<std::string> MerchantService::getAllowedService()
{
	return DataSource::getDataAccessInstance()->getAllowedService();
}

void MerchantService::updateService(std::list<std::string> appliances, double distance)
{
	ThreadPool::getInstance().submit(&DataAccess::updateMerchantServices, DataSource::getDataAccessInstance(), _merchant->id(), appliances, distance);
}
