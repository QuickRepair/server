#include "usermanagesystem.h"
#include "../../network/jsonAnalyser.h"

using std::string;

UserManageSystem &UserManageSystem::Init()
{
    static UserManageSystem user_manage_system;
    return user_manage_system;
}

std::string UserManageSystem::shunt_message(const std::string &data)
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

UserManageSystem::UserManageSystem()
{

}
