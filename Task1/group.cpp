#include "group.h"
#include <algorithm>

Group::Group() : id(""), name("") {}

Group::Group(std::string id, std::string name) : id(id), name(name) {}

std::string Group::get_id() const { return id; }

std::string Group::get_name() const { return name; }

std::vector<std::string> Group::get_user_ids() const { return user_ids; }

void Group::add_user(const std::string &user_id) { user_ids.push_back(user_id); }

void Group::remove_user(const std::string &user_id)
{
    user_ids.erase(std::remove(user_ids.begin(), user_ids.end(), user_id), user_ids.end());
}

std::string Group::get_group_info() const
{
    std::string result = "Group ID: " + id + ", Name: " + name + ", Users: ";
    for (const auto &uid : user_ids)
        result += uid + " ";
    return result;
}