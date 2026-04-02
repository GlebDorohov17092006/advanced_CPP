#include "user.h"

User::User() : user_name(""), id(""), meta_information(""), group_id("") {}

User::User(std::string user_name, std::string id,
           std::string meta_information, std::string group_id)
    : user_name(user_name), id(id), meta_information(meta_information), group_id(group_id) {}

std::string User::get_id() const { return id; }

std::string User::get_group_id() const { return group_id; }

std::string User::get_user_info() const
{
    return "ID: " + id + ", Name: " + user_name + ", Meta: " + meta_information + ", Group: " + group_id;
}

void User::set_group_id(const std::string &new_group_id) { group_id = new_group_id; }