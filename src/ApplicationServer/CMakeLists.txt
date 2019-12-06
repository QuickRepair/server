cmake_minimum_required(VERSION 3.7)
project(QuickRepairApplicationServer)

set(CMAKE_CXX_STANDARD 17)

# set directories, use grpc lib under directory third_party/
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_HOME_DIRECTORY}/third_party/grpc/include
        ${CMAKE_HOME_DIRECTORY}/third_party/grpc/third_party/protobuf/src
)
link_directories(
        ${CMAKE_CURRENT_BINARY_DIR}/../grpc
        ${CMAKE_CURRENT_BINARY_DIR}/../libpqxx
)
#find_package(Boost REQUIRED)
#find_package(cpprestsdk CONFIG REQUIRED)

option(_TEST_ENV "Switch to test enviroment" OFF)
if(_TEST_ENV STREQUAL ON)
    message(STATUS "In test enviroment")
    add_definitions(-DENV_TEST)
    find_package(GTest MODULE REQUIRED)
    link_libraries(GTest::GTest)
endif()

# add grpc
add_subdirectory(${CMAKE_HOME_DIRECTORY}/third_party/grpc ${CMAKE_CURRENT_BINARY_DIR}/../grpc EXCLUDE_FROM_ALL)
add_subdirectory(${CMAKE_HOME_DIRECTORY}/third_party/libpqxx ${CMAKE_CURRENT_BINARY_DIR}/..//libpqxx EXCLUDE_FROM_ALL)

# generate grpc files
set(_PROTOC_INCLUDE_PATH ${CMAKE_HOME_DIRECTORY}/third_party/grpc/third_party/protobuf/src/)
set(_GRPC_CPP_PLUGIN_EXECUTABLE ${CMAKE_CURRENT_BINARY_DIR}/../grpc/grpc_cpp_plugin)
set(_PROTOC_PATH ${CMAKE_CURRENT_BINARY_DIR}/../grpc/third_party/protobuf/protoc)
set(GRPC_OUT QuickRepair.grpc.pb.h QuickRepair.grpc.pb.cc QuickRepair.pb.h QuickRepair.pb.cc)
add_custom_command(
        OUTPUT ${GRPC_OUT}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND ${_PROTOC_PATH} -I${_PROTOC_INCLUDE_PATH} -I../../protos --cpp_out=${CMAKE_CURRENT_BINARY_DIR} ../../protos/QuickRepair.proto
        COMMAND ${_PROTOC_PATH} -I${_PROTOC_INCLUDE_PATH} -I../../protos --grpc_out=${CMAKE_CURRENT_BINARY_DIR} --plugin=protoc-gen-grpc=${_GRPC_CPP_PLUGIN_EXECUTABLE} ../../protos/QuickRepair.proto
        USES_TERMINAL
        COMMENT "generate c++ grpc files"
        DEPENDS ../../protos/QuickRepair.proto protoc grpc_cpp_plugin grpc_python_plugin
)
# ignore generated file compile warning
set_source_files_properties(${GRPC_OUT} PROPERTIES COMPILE_FLAGS -w)

add_executable(${PROJECT_NAME} main.cpp
        ${GRPC_OUT}
        Order/Order.cpp Order/Order.h
        Order/OrderStates/OrderState.h Order/OrderStates/AcceptableOrderPriceRange.cpp Order/OrderStates/AcceptableOrderPriceRange.h Order/OrderStates/OrderEvaluate.cpp Order/OrderStates/OrderEvaluate.h

        Account/ContactInformation.cpp Account/ContactInformation.h Account/MerchantService.cpp Account/MerchantService.h Account/Account.cpp Account/Account.h

        DataSource/MariaDB/MariaDBConnection.cpp DataSource/MariaDB/MariaDBConnection.h DataSource/MariaDB/QueryResult.cpp DataSource/MariaDB/QueryResult.h Factories/OrderFactory.cpp Factories/OrderFactory.hpp
        Factories/AccountFactory.h
        Factories/AccountFactory.cpp DataSource/DataAccess.h DataSource/SimulateDatabase/SimulateDatabase.cpp DataSource/SimulateDatabase/SimulateDatabase.h

        Errors/DatabaseInternalError.hpp Errors/QueryResultEmptyError.hpp Errors/NoSuchAnAccountOrPasswordNotRightError.hpp Errors/AccountAlreadyExistError.hpp Errors/OrderNotAtRightState.hpp Errors/NoAuthenticationCarrierFound.hpp

        Configuration/Configure.cpp Configuration/Configure.h

        ManagerProxy/Subjects/AccountManagerSubject.h ManagerProxy/Subjects/OrderManagerSubject.h
        ManagerProxy/AccountManagerProxy.cpp ManagerProxy/AccountManagerProxy.h ManagerProxy/OrderManagerProxy.cpp ManagerProxy/OrderManagerProxy.h
        ManagerProxy/Subjects/Managers/AccountManager.cpp ManagerProxy/Subjects/Managers/AccountManager.h ManagerProxy/Subjects/Managers/OrderManager.cpp ManagerProxy/Subjects/Managers/OrderManager.h
        ManagerProxy/AuthenticationCarrier/AuthenticationSaver.cpp ManagerProxy/AuthenticationCarrier/AuthenticationSaver.h ManagerProxy/AuthenticationCarrier/AuthenticationCarrier.h ManagerProxy/AuthenticationCarrier/AuthenticationToScreen.cpp ManagerProxy/AuthenticationCarrier/AuthenticationToScreen.h DataSource/Postgres/PostgresConnection.cpp DataSource/Postgres/PostgresConnection.h Factories/DataAccessFactory.cpp Factories/DataAccessFactory.h DataSource/DataSource.hpp Errors/InitError.hpp DataSource/DataSource.cpp ManagerProxy/Subjects/TokenManagerSubject.h ManagerProxy/TokenManagerProxy.cpp ManagerProxy/TokenManagerProxy.h Token/Token.cpp Token/Token.h ManagerProxy/Subjects/Managers/TokenManager.cpp ManagerProxy/Subjects/Managers/TokenManager.h Order/OrderStates/OrderState.cpp DataSource/DataAccess.cpp ManagerProxy/AuthenticationCarrier/AuthenticationCarrier.cpp ManagerProxy/Subjects/AccountManagerSubject.cpp ManagerProxy/Subjects/OrderManagerSubject.cpp ManagerProxy/Subjects/TokenManagerSubject.cpp RPC/RPCServer.cpp RPC/RPCServer.h RPC/RPCCall.cpp RPC/RPCCall.h
        types.h Thread/ThreadPool.cpp Thread/ThreadPool.hpp Thread/ThreadBase.cpp Thread/ThreadBase.hpp)

# need grpc++ lib
add_dependencies(${PROJECT_NAME} grpc++)
add_dependencies(${PROJECT_NAME} pqxx_shared)

target_link_libraries(${PROJECT_NAME} pqxx grpc++)
