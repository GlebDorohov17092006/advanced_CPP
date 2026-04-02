#pragma once

#include "user.h"
#include <map>
#include <string>

class Users
{
private:
    std::map<std::string, User> users;

public:
    void add_user(const User &user);
    void delete_user(const std::string &user_id);
    User *get_user(const std::string &user_id);
    std::string all_users_info() const;
};