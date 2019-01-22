#ifndef ACCOUNTREGISTERMANAGER_H
#define ACCOUNTREGISTERMANAGER_H

#include <string>

class AccountRegisterManager
{
public:
    static AccountRegisterManager &Init();
    std::string shunt_message(const std::string &data);
private:
    AccountRegisterManager();

    std::string request_register();
    std::string request_checkAccount();
};

#endif // ACCOUNTREGISTERMANAGER_H
