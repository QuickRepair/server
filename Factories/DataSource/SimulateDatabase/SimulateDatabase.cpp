//
// Created by gaojian on 19-4-13.
//

#include "SimulateDatabase.h"
#include "Factories/DataSource/OrderStateParameters.h"

unsigned long SimulateDatabase::createOrder(unsigned long committerId, unsigned long acceptorId, std::string applianceType, std::string detail)
{
}

std::vector<std::tuple<unsigned long, unsigned long, unsigned long, std::string, std::string, unsigned long>> SimulateDatabase::queryOrderByAccountId(unsigned long id)
{}

std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>
SimulateDatabase::queryOrderStateByOrderIdAndStateId(unsigned long orderId, unsigned long stateId)
{}

std::tuple<std::shared_ptr<OrderStateAbstractFactory>, OrderStateParameters>
SimulateDatabase::queryOrderStateByOrderIdAndLastStateId(unsigned long orderId, unsigned long lastState)
{}

unsigned long SimulateDatabase::checkMerchantPasswordAndGetId(std::string account, std::string password)
{}

unsigned long SimulateDatabase::checkCustomerPasswordAndGetId(std::string account, std::string password)
{}

std::vector<std::tuple<>> SimulateDatabase::queryContactInfoByUserId(unsigned long userId)
{}

void SimulateDatabase::createMerchantAccount(std::string account, std::string password)
{}

void SimulateDatabase::createCustomerAccount(std::string account, std::string password)
{}

void SimulateDatabase::updateMerchantAccountPassword(std::string account, std::string password)
{}

void SimulateDatabase::updateCustomerAccountPassword(std::string account, std::string password)
{}

std::tuple<std::list<std::string>, int> SimulateDatabase::queryMerchantServiceType(unsigned long id)
{}

std::tuple<std::string, std::string> SimulateDatabase::loadMerchant(unsigned long id)
{}

std::tuple<std::string, std::string> SimulateDatabase::loadCustomer(unsigned long id)
{}