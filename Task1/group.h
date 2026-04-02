#pragma once

#include <string>
#include <vector>

class Group
{
private:
    std::string id;
    std::string name;
    std::vector<std::string> user_ids;

public:
    Group();
    Group(std::string id, std::string name);
    std::string get_id() const;
    std::string get_name() const;
    std::vector<std::string> get_user_ids() const;
    void add_user(const std::string &user_id);
    void remove_user(const std::string &user_id);
    std::string get_group_info() const;
};