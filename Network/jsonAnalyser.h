#ifndef DATASHUNT_H
#define DATASHUNT_H

#include <map>
#include <memory>
#include <string>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

struct JSONDATA{
    std::string TYPE="type";
    std::string DATE="date";
    std::string DATA="data";
};

struct JSONORDER{
    std::string ID="id";
    std::string DATE="date";
    std::string PRICERANGE="priceRange";
    std::string TRANSACTION="tannsaction";
    std::string EVALUATE="evaluate";
    std::string DETAILS="details";
    std::string ADDRESS="address";
    int StartRepair=0;
    int EndRepair=1;
    int ReceiveOrder=2;
    int UploadOrder=3;
    int PayOrder=4;
    int AcceptOrder=5;
};

struct JSONUSER{
    std::string ID="id";
    std::string PASSWD="password";
};

struct JSONMESSAGE{
    std::string Id="id";
    std::string FROMUSER="fromUser";
    std::string TOUSER="toUser";
};

class NetworkConnection;

class JsonAnalyser
{
public:
    static JsonAnalyser &Init();
    static JSONDATA &JsonData();
    static JSONUSER &JsonUser();
    static JSONORDER &JsonOrder();
    static JSONMESSAGE &JsonMessage();

    std::string shunt_message(std::shared_ptr<NetworkConnection> socket, const std::string &data);

    std::string json_set(const std::string &flag, const std::string &value, const std::string &data="{}");
    std::string json_get(const std::string &flag, const std::string &data);

private:
    JsonAnalyser();
    std::string getTime();

    time_t timep;
    std::map<unsigned int, std::shared_ptr<NetworkConnection>> m_connects;
};

#endif // DATASHUNT_H
