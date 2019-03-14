cmake_minimum_required(VERSION 3.13)
project(HARserver)

set(CMAKE_CXX_STANDARD 17)
include_directories(/usr/local/mariadb/mariadb-connector-c/include)
link_directories(/usr/local/mariadb/mariadb-connector-c/lib/mariadb)

add_executable(HARserver main.cpp
        Order/Order.cpp Order/Order.h Order/OrderStates/OrderState.h Order/OrderStates/OrderUnreceivedState.cpp Order/OrderStates/OrderUnreceivedState.h Order/OrderStates/OrderReceivedState.cpp Order/OrderStates/OrderReceivedState.h Order/OrderStates/OrderStartRepairState.cpp Order/OrderStates/OrderStartRepairState.h Order/OrderStates/OrderEndRepairState.cpp Order/OrderStates/OrderEndRepairState.h Order/OrderStates/OrderEvaluate.cpp Order/OrderStates/OrderEvaluate.h
        Managers/OrderManager.cpp Managers/OrderManager.h
        Order/OrderStates/AcceptableOrderPriceRange.cpp Order/OrderStates/AcceptableOrderPriceRange.h Order/OrderStates/OrderFinishedState.cpp Order/OrderStates/OrderFinishedState.h Account/ContactInformation.cpp Account/ContactInformation.h Account/CustomerAccount.cpp Account/CustomerAccount.h Account/MerchantAccount.cpp Account/MerchantAccount.h Account/MerchantServiceType.cpp Account/MerchantServiceType.h Account/Account.cpp Account/Account.h
        Factories/Database/DatabaseConnection.cpp Factories/Database/DatabaseConnection.h Factories/Database/QueryResult.cpp Factories/Database/QueryResult.h Factories/OrderFactory.cpp Factories/OrderFactory.h Factories/OrderStateFactories/OrderStateAbstractFactory.h Factories/Database/OrderStateParameters.h Factories/OrderStateFactories/OrderUnreceivedStateFactory.cpp Factories/OrderStateFactories/OrderUnreceivedStateFactory.h Factories/OrderStateFactories/OrderReceivedStateFactory.cpp Factories/OrderStateFactories/OrderReceivedStateFactory.h Factories/OrderStateFactories/OrderStartRepairStateFactory.cpp Factories/OrderStateFactories/OrderStartRepairStateFactory.h Factories/OrderStateFactories/OrderEndRepairStateFactory.cpp Factories/OrderStateFactories/OrderEndRepairStateFactory.h Factories/OrderStateFactories/OrderFinishedStateFactory.cpp Factories/OrderStateFactories/OrderFinishedStateFactory.h Factories/UserFactory.hpp
        Network/networkconnection.cpp Network/networkconnection.h Network/networkserver.cpp Network/networkserver.h Network/jsonAnalyser.cpp Network/jsonAnalyser.h Managers/AccountManager.cpp Managers/AccountManager.hpp Errors/DatabaseInternalError.cpp Errors/DatabaseInternalError.h Errors/ResultEmpty.cpp Errors/ResultEmpty.h Factories/OrderStateFactories/OrderStateAbstractFactory.cpp Order/OrderStates/OrderState.cpp Errors/PasswordNotRight.cpp Errors/PasswordNotRight.h Errors/NoSuchAnAccount.cpp Errors/NoSuchAnAccount.h)

target_link_libraries(HARserver -lmariadb -lpthread -lboost_system)