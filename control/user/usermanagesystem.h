#ifndef USERMANAGESYSTEM_H
#define USERMANAGESYSTEM_H

#include <string>

class UserManageSystem
{
public:
    static UserManageSystem &Init();
    std::string shunt_message(const std::string &data);
private:
    UserManageSystem();

    std::string online_state();
    std::string confirm_state();

    std::string request_login();
    std::string request_logout();

    std::string request_register();
    std::string confirm_register();

    std::string confirm_checkAccount();
    std::string request_changeAccount();
};

#endif // USERMANAGESYSTEM_H
