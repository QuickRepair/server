#include "accountonlinemanager.h"
#include "../../network/jsonAnalyser.h"

using std::string;

AccountOnlineManager &AccountOnlineManager::Init()
{
    static AccountOnlineManager account_online_manager;
    return account_online_manager;
}

string AccountOnlineManager::shunt_message(const string &data)
{
    string str;
    int type=atoi(JsonAnalyser::Init().json_get(JsonAnalyser::JsonData().TYPE, data).data());
    switch(type){
    case 0:
        str="number 1";
        break;
    case 2:
        str="number 2";
        break;
    default:
        str="unknow command";
        break;
    }
    str=JsonAnalyser::Init().json_set(JsonAnalyser::JsonData().DATA, str);
    str=JsonAnalyser::Init().json_set(JsonAnalyser::JsonData().TYPE, "0");
    return str;
}

AccountOnlineManager::AccountOnlineManager()
{

}
