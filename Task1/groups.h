#pragma once

#include <vector>
#include <map>
#include <string>
#include "group.h"

class Groups
{
private:
    std::map<std::string, Group> groups;

public:
    void add_group(const Group &group);
    void delete_group(const std::string &id);
    Group *get_group(const std::string &id);
    std::string all_groups_info() const;
};