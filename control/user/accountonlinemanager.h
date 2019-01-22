#ifndef ACCOUNTONLINEMANAGER_H
#define ACCOUNTONLINEMANAGER_H

#include <string>

class AccountOnlineManager
{
public:
    static AccountOnlineManager &Init();
    std::string shunt_message(const std::string &data);
private:
    AccountOnlineManager();

    std::string request_login();
    std::string request_logout();
};

#endif // ACCOUNTONLINEMANAGER_H
