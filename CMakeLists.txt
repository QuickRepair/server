cmake_minimum_required(VERSION 3.7)
project(HARserver)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} $ENV{PATH})
link_directories($ENV{PATH})

find_package(Boost REQUIRED)
find_package(cpprestsdk CONFIG REQUIRED)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(LINUX 1)
    message(STATUS "build for platform linux")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(WINDOWS 1)
    message(STATUS "Build for platform windows")
endif()

option(_TEST_ENV "Switch to test enviroment" OFF)
if(_TEST_ENV STREQUAL ON)
    message(STATUS "In test enviroment")
    add_definitions(-DENV_TEST)
    find_package(GTest MODULE REQUIRED)
    link_libraries(GTest::GTest)
endif()

add_executable(HARserver main.cpp
        Order/Order.cpp Order/Order.h
        Order/OrderStates/OrderState.h Order/OrderStates/AcceptableOrderPriceRange.cpp Order/OrderStates/AcceptableOrderPriceRange.h Order/OrderStates/OrderEvaluate.cpp Order/OrderStates/OrderEvaluate.h
        Order/OrderStates/OrderUnreceivedState.cpp Order/OrderStates/OrderUnreceivedState.h Order/OrderStates/OrderReceivedState.cpp Order/OrderStates/OrderReceivedState.h Order/OrderStates/OrderStartRepairState.cpp Order/OrderStates/OrderStartRepairState.h Order/OrderStates/OrderEndRepairState.cpp Order/OrderStates/OrderEndRepairState.h
        Order/OrderStates/OrderFinishedState.cpp Order/OrderStates/OrderFinishedState.h Order/OrderStates/OrderRejectedState.cpp Order/OrderStates/OrderRejectedState.h

        Account/ContactInformation.cpp Account/ContactInformation.h Account/CustomerAccount.cpp Account/CustomerAccount.h Account/MerchantAccount.cpp Account/MerchantAccount.h Account/MerchantService.cpp Account/MerchantService.h Account/Account.cpp Account/Account.h

        DataSource/MariaDB/MariaDBConnection.cpp DataSource/MariaDB/MariaDBConnection.h DataSource/MariaDB/QueryResult.cpp DataSource/MariaDB/QueryResult.h Factories/OrderFactory.cpp Factories/OrderFactory.hpp
        Factories/AccountFactory.h
        Factories/MerchantFactory.cpp Factories/MerchantFactory.h Factories/AccountFactory.cpp Factories/CustomerFactory.cpp DataSource/DataAccess.h DataSource/SimulateDatabase/SimulateDatabase.cpp DataSource/SimulateDatabase/SimulateDatabase.h

        Errors/DatabaseInternalError.hpp Errors/QueryResultEmptyError.hpp Errors/NoSuchAnAccountOrPasswordNotRightError.hpp Errors/AccountAlreadyExistError.hpp Factories/CustomerFactory.h Errors/OrderNotAtRightState.hpp Errors/NoAuthenticationCarrierFound.hpp

        Configuration/Configure.cpp Configuration/Configure.h

        RestServer/RestHandler.cpp RestServer/RestHandler.h

        TestCases/OrderProcessTest.cpp TestCases/FactoryTest.cpp TestCases/ConfigParseTest.cpp TestCases/ManagerTest.cpp TestCases/OrderTest.cpp

        ManagerProxy/Subjects/AccountManagerSubject.h ManagerProxy/Subjects/OrderManagerSubject.h
        ManagerProxy/AccountManagerProxy.cpp ManagerProxy/AccountManagerProxy.h ManagerProxy/OrderManagerProxy.cpp ManagerProxy/OrderManagerProxy.h
        ManagerProxy/Subjects/Managers/AccountManager.cpp ManagerProxy/Subjects/Managers/AccountManager.h ManagerProxy/Subjects/Managers/OrderManager.cpp ManagerProxy/Subjects/Managers/OrderManager.h
        RestServer/RequestInterpreter.cpp RestServer/RequestInterpreter.h
        ManagerProxy/AuthenticationCarrier/AuthenticationSaver.cpp ManagerProxy/AuthenticationCarrier/AuthenticationSaver.h ManagerProxy/AuthenticationCarrier/AuthenticationCarrier.h ManagerProxy/AuthenticationCarrier/AuthenticationToScreen.cpp ManagerProxy/AuthenticationCarrier/AuthenticationToScreen.h DataSource/Postgres/PostgresConnection.cpp DataSource/Postgres/PostgresConnection.h Factories/DataAccessFactory.cpp Factories/DataAccessFactory.h DataSource/DataSource.hpp Errors/InitError.hpp DataSource/DataSource.cpp RestServer/RespondGenerator.cpp RestServer/RespondGenerator.h ManagerProxy/Subjects/TokenManagerSubject.h ManagerProxy/TokenManagerProxy.cpp ManagerProxy/TokenManagerProxy.h Token/Token.cpp Token/Token.h ManagerProxy/Subjects/Managers/TokenManager.cpp ManagerProxy/Subjects/Managers/TokenManager.h Order/OrderStates/OrderState.cpp DataSource/DataAccess.cpp ManagerProxy/AuthenticationCarrier/AuthenticationCarrier.cpp ManagerProxy/Subjects/AccountManagerSubject.cpp ManagerProxy/Subjects/OrderManagerSubject.cpp "ManagerProxy/Subjects/ TokenManagerSubject.cpp")

target_link_libraries(HARserver pqxx cpprestsdk::cpprest boost_system)
