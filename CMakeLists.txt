cmake_minimum_required(VERSION 3.13)
project(HAR)

set(CMAKE_CXX_STANDARD 17)
include_directories(/usr/local/mariadb/mariadb-connector-c/include)
link_directories(/usr/local/mariadb/mariadb-connector-c/lib/mariadb)

add_executable(HAR main.cpp
        entity/order/order.cpp entity/order/order.h entity/order/orderstate.h entity/order/orderunreceivedstate.cpp entity/order/orderunreceivedstate.h entity/order/orderreceivedstate.cpp entity/order/orderreceivedstate.h entity/order/orderstartrepairstate.cpp entity/order/orderstartrepairstate.h entity/order/orderendrepairstate.cpp entity/order/orderendrepairstate.h entity/order/evaluate.cpp entity/order/evaluate.h
        control/user/accountregistermanager.cpp control/user/accountregistermanager.h control/user/usermanagesystem.cpp control/user/usermanagesystem.h control/user/accountonlinemanager.cpp control/user/accountonlinemanager.h control/user/register.cpp control/user/register.h control/order/ordermanagesystem.cpp control/order/ordermanagesystem.h
        entity/order/orderpricerange.cpp entity/order/orderpricerange.h entity/order/orderfinishedstate.cpp entity/order/orderfinishedstate.h entity/user/addressinformation.cpp entity/user/addressinformation.h entity/user/customeraccount.cpp entity/user/customeraccount.h entity/user/merchantaccount.cpp entity/user/merchantaccount.h entity/user/merchantservertype.cpp entity/user/merchantservertype.h entity/user/user.cpp entity/user/user.h
        database/databaseconnection.cpp database/databaseconnection.h database/queryresult.cpp database/queryresult.h database/orderfactory.cpp database/orderfactory.h database/orderstateabstractfactory.h database/orderstateparameters.h database/orderunreceivedstatefactory.cpp database/orderunreceivedstatefactory.h database/orderreceivedstatefactory.cpp database/orderreceivedstatefactory.h database/orderstartrepairstatefactory.cpp database/orderstartrepairstatefactory.h database/orderendrepairstatefactory.cpp database/orderendrepairstatefactory.h database/orderfinishedstatefactory.cpp database/orderfinishedstatefactory.h database/userfactory.hpp
        network/networkconnection.cpp network/networkconnection.h network/networkserver.cpp network/networkserver.h network/jsonAnalyser.cpp network/jsonAnalyser.h)

target_link_libraries(HAR -lmariadb -lpthread -lboost_system)