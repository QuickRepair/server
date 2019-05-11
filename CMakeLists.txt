cmake_minimum_required(VERSION 3.7)
project(HARserver)

set(CMAKE_CXX_STANDARD 17)

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
        Order/OrderStates/OrderFinishedState.cpp Order/OrderStates/OrderFinishedState.h Order/OrderStates/OrderState.cpp Order/OrderStates/OrderRejectedState.cpp Order/OrderStates/OrderRejectedState.h

        Account/ContactInformation.cpp Account/ContactInformation.h Account/CustomerAccount.cpp Account/CustomerAccount.h Account/MerchantAccount.cpp Account/MerchantAccount.h Account/MerchantServiceType.cpp Account/MerchantServiceType.h Account/Account.cpp Account/Account.h

        Factories/DataSource/MariaDB/MariaDBConnection.cpp Factories/DataSource/MariaDB/MariaDBConnection.h Factories/DataSource/MariaDB/QueryResult.cpp Factories/DataSource/MariaDB/QueryResult.h Factories/OrderFactory.cpp Factories/OrderFactory.h
        Factories/OrderStateFactories/OrderStateAbstractFactory.h Factories/DataSource/OrderStateParameters.h Factories/OrderStateFactories/OrderUnreceivedStateFactory.cpp Factories/OrderStateFactories/OrderUnreceivedStateFactory.h Factories/OrderStateFactories/OrderReceivedStateFactory.cpp Factories/OrderStateFactories/OrderReceivedStateFactory.h Factories/OrderStateFactories/OrderStartRepairStateFactory.cpp Factories/OrderStateFactories/OrderStartRepairStateFactory.h Factories/OrderStateFactories/OrderEndRepairStateFactory.cpp Factories/OrderStateFactories/OrderEndRepairStateFactory.h Factories/OrderStateFactories/OrderFinishedStateFactory.cpp Factories/OrderStateFactories/OrderFinishedStateFactory.h Factories/AccountFactory.h
        Factories/OrderStateFactories/OrderStateAbstractFactory.cpp Factories/MerchantFactory.cpp Factories/MerchantFactory.h Factories/AccountFactory.cpp Factories/CustomerFactory.cpp Factories/DataSource/DataSource.hpp Factories/DataSource/SimulateDatabase/SimulateDatabase.cpp Factories/DataSource/SimulateDatabase/SimulateDatabase.h

        Errors/DatabaseInternalError.hpp Errors/QueryResultEmptyError.hpp Errors/PasswordNotRightError.hpp Errors/NoSuchAnAccountError.hpp Errors/AccountAlreadyExistError.hpp Factories/CustomerFactory.h Errors/OrderNotAtRightState.hpp Errors/NoAuthenticationCarrierFound.hpp

        Configuration/Configure.cpp Configuration/Configure.h

        RestServer/RestHandler.cpp RestServer/RestHandler.h

        test/OrderProcessTest.cpp test/FactoryTest.cpp test/ConfigParseTest.cpp test/ManagerTest.cpp test/OrderTest.cpp

        ManagerProxy/Subjects/AccountManagerSubject.cpp ManagerProxy/Subjects/AccountManagerSubject.h ManagerProxy/Subjects/OrderManagerSubject.cpp ManagerProxy/Subjects/OrderManagerSubject.h
        ManagerProxy/AccountManagerProxy.cpp ManagerProxy/AccountManagerProxy.h ManagerProxy/OrderManagerProxy.cpp ManagerProxy/OrderManagerProxy.h
        ManagerProxy/Managers/AccountManager.cpp ManagerProxy/Managers/AccountManager.h ManagerProxy/Managers/OrderManager.cpp ManagerProxy/Managers/OrderManager.h
        ManagerProxy/InstructionsAnalyser.cpp ManagerProxy/InstructionsAnalyser.h
        ManagerProxy/AuthenticationCarrier/AuthenticationSaver.cpp ManagerProxy/AuthenticationCarrier/AuthenticationSaver.h ManagerProxy/AuthenticationCarrier/AuthenticationCarrier.cpp ManagerProxy/AuthenticationCarrier/AuthenticationCarrier.h ManagerProxy/AuthenticationCarrier/AuthenticationToScreen.cpp ManagerProxy/AuthenticationCarrier/AuthenticationToScreen.h Factories/DataSource/Postgres/PostgresConnection.cpp Factories/DataSource/Postgres/PostgresConnection.h Factories/OrderStateFactories/OrderRejectedStateFactory.cpp Factories/OrderStateFactories/OrderRejectedStateFactory.h)

target_link_libraries(HARserver pqxx cpprestsdk::cpprest)
