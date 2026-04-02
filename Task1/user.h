#pragma once

#include <string>

class User
{
private:
    std::string user_name;
    std::string id;
    std::string meta_information;
    std::string group_id;

public:
    User();
    User(std::string user_name, std::string id,
         std::string meta_information, std::string group_id);
    std::string get_id() const;
    std::string get_group_id() const;
    std::string get_user_info() const;
    void set_group_id(const std::string &new_group_id);
};