//
// Created by gaojian on 19-3-14.
//

#ifndef HARSERVER_PASSWORDNOTRIGHT_H
#define HARSERVER_PASSWORDNOTRIGHT_H

#include <exception>
#include <string>

class PasswordNotRight : public std::exception {
public:
    explicit PasswordNotRight(std::string error);
    ~PasswordNotRight() override = default;

    const char* what() const noexcept override;

private:
    std::string m_msg;
};


#endif //HARSERVER_PASSWORDNOTRIGHT_H
