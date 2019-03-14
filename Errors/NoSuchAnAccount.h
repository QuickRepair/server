//
// Created by gaojian on 19-3-14.
//

#ifndef HARSERVER_NOSUCHANACCOUNT_H
#define HARSERVER_NOSUCHANACCOUNT_H

#include <exception>
#include <string>

class NoSuchAnAccount : public std::exception {
public:
    explicit NoSuchAnAccount(std::string error);
    ~NoSuchAnAccount() override = default;

    const char* what() const noexcept override;

private:
    std::string m_msg;
};


#endif //HARSERVER_NOSUCHANACCOUNT_H
