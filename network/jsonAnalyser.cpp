#include "jsonAnalyser.h"
#include <iostream>

using std::cout;
using std::endl;
using std::atoi;
using std::string;
using std::to_string;
using std::shared_ptr;
using std::stringstream;

using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::write_json;
using boost::property_tree::ptree_error;

JsonAnalyser::JsonAnalyser()
{
}

JsonAnalyser &JsonAnalyser::Init()
{
    static JsonAnalyser data_shunt;
    return data_shunt;
}

JSONDATA &JsonAnalyser::JsonData()
{
    static JSONDATA json_data;
    return json_data;
}

JSONUSER &JsonAnalyser::JsonUser()
{
    static JSONUSER json_user;
    return json_user;
}

JSONORDER &JsonAnalyser::JsonOrder()
{
    static JSONORDER json_order;
    return json_order;
}

JSONMESSAGE &JsonAnalyser::JsonMessage()
{
    static JSONMESSAGE json_message;
    return json_message;
}

string JsonAnalyser::shunt_message(std::shared_ptr<NetworkConnection> socket, const string &data)
{
    string str;
    int type=-1;
    try{
        type=atoi(json_get(JsonData().TYPE, data).data());
    }catch(ptree_error &e){
        cout<<"shunt_message:"<<e.what()<<endl;
        type=-1;
    }

    switch(type){
    case 0:
        str="number 0";
        break;
    case 1:
        str="number 1";
        break;
    default:
        str="unknow command";
        break;
    }
    str=json_set(JsonData().DATA, str);
    str=json_set(JsonData().DATE, getTime(), str);
    str=json_set(JsonData().TYPE, "0", str);
    return str;
}

string JsonAnalyser::json_set(const string &flag, const string &value, const string &data)
{
    stringstream s1(data);
    ptree json;
    try{
        read_json(s1, json);
        json.put(flag, value);
    }catch(ptree_error &e) {
        cout<<"json_set:"<<e.what()<<endl;
    }
    stringstream s2;
    write_json(s2, json);
    return string(s2.str());
}

string JsonAnalyser::json_get(const std::string &flag, const string &data)
{
    string str;
    stringstream s1(data);
    try{
        ptree json;
        read_json(s1, json);
        str=json.get<string>(flag);
    }catch(ptree_error &e){
        throw e;
    }
    return str;
}

string JsonAnalyser::getTime()
{
    time(&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep));
    return string(tmp);
}
