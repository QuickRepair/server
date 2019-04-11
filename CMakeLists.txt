cmake_minimum_required(VERSION 3.7)
project(HARserver)

set(CMAKE_CXX_STANDARD 11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} $ENV{PATH})
link_directories($ENV{PATH})

find_package(Boost REQUIRED)
find_package(cpprestsdk CONFIG REQUIRED)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(LINUX 1)
    message(STATUS "build for platform linux")
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(WINDOWS 1)
    message(STATUS "build for platform windows")
endif()

option(_TEST_ENV "Switch to test enviroment" OFF)
if(_TEST_ENV STREQUAL ON)
    message("In test enviroment")
    add_definitions(-DENV_TEST)
    find_package(GTest MODULE REQUIRED)
    link_libraries(GTest::GTest)
endif()

add_executable(HARserver main.cpp
        Order/Order.cpp Order/Order.h Order/OrderStates/OrderState.h Order/OrderStates/OrderUnreceivedState.cpp Order/OrderStates/OrderUnreceivedState.h Order/OrderStates/OrderReceivedState.cpp Order/OrderStates/OrderReceivedState.h Order/OrderStates/OrderStartRepairState.cpp Order/OrderStates/OrderStartRepairState.h Order/OrderStates/OrderEndRepairState.cpp Order/OrderStates/OrderEndRepairState.h Order/OrderStates/OrderEvaluate.cpp Order/OrderStates/OrderEvaluate.h
        Managers/OrderManager.cpp Managers/OrderManager.h
        Order/OrderStates/AcceptableOrderPriceRange.cpp Order/OrderStates/AcceptableOrderPriceRange.h Order/OrderStates/OrderFinishedState.cpp Order/OrderStates/OrderFinishedState.h Account/ContactInformation.cpp Account/ContactInformation.h Account/CustomerAccount.cpp Account/CustomerAccount.h Account/MerchantAccount.cpp Account/MerchantAccount.h Account/MerchantServiceType.cpp Account/MerchantServiceType.h Account/Account.cpp Account/Account.h
        Factories/Database/DatabaseConnection.cpp Factories/Database/DatabaseConnection.h Factories/Database/QueryResult.cpp Factories/Database/QueryResult.h Factories/OrderFactory.cpp Factories/OrderFactory.h Factories/OrderStateFactories/OrderStateAbstractFactory.h Factories/Database/OrderStateParameters.h Factories/OrderStateFactories/OrderUnreceivedStateFactory.cpp Factories/OrderStateFactories/OrderUnreceivedStateFactory.h Factories/OrderStateFactories/OrderReceivedStateFactory.cpp Factories/OrderStateFactories/OrderReceivedStateFactory.h Factories/OrderStateFactories/OrderStartRepairStateFactory.cpp Factories/OrderStateFactories/OrderStartRepairStateFactory.h Factories/OrderStateFactories/OrderEndRepairStateFactory.cpp Factories/OrderStateFactories/OrderEndRepairStateFactory.h Factories/OrderStateFactories/OrderFinishedStateFactory.cpp Factories/OrderStateFactories/OrderFinishedStateFactory.h Factories/AccountFactory.h
        Managers/AccountManager.cpp Managers/AccountManager.h Errors/DatabaseInternalError.cpp Errors/DatabaseInternalError.h Errors/QueryResultEmptyError.cpp Errors/QueryResultEmptyError.h Factories/OrderStateFactories/OrderStateAbstractFactory.cpp Order/OrderStates/OrderState.cpp Errors/PasswordNotRightError.cpp Errors/PasswordNotRightError.h Errors/NoSuchAnAccountError.cpp Errors/NoSuchAnAccountError.h RestServer/RestHandler.cpp RestServer/RestHandler.h Managers/InstructionsAnalyser.cpp Managers/InstructionsAnalyser.h Managers/AuthenticationCarrier/AuthenticationCarrier.cpp Managers/AuthenticationCarrier/AuthenticationCarrier.h Managers/AuthenticationCarrier/AuthenticationToScreen.cpp Managers/AuthenticationCarrier/AuthenticationToScreen.h Errors/AccountAlreadyExistError.cpp Errors/AccountAlreadyExistError.h Configuration/Configure.cpp Configuration/Configure.h Factories/MerchantFactory.cpp Factories/MerchantFactory.h Factories/AccountFactory.cpp Factories/CustomerFactory.cpp Factories/CustomerFactory.h Order/OrderStates/OrderRejectedState.cpp Order/OrderStates/OrderRejectedState.h Errors/OrderNotAtRightState.cpp Errors/OrderNotAtRightState.h test/OrderProcess.cpp)

if(WINDOWS)
    target_link_libraries(HARserver libmariadb cpprestsdk::cpprest)
elseif(LINUX)
    target_link_libraries(HARserver mariadb crypto ssl cpprest)
endif()
