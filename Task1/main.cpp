#include <iostream>
#include <sstream>
#include <string>
#include "user.h"
#include "group.h"
#include "users.h"
#include "groups.h"

int main()
{
    Users users;
    Groups groups;
    std::string command;

    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, command);
        std::istringstream iss(command);
        std::string cmd;
        iss >> cmd;

        if (cmd == "createUser")
        {
            std::string id, name, meta, group_id;
            iss >> id >> name >> meta >> group_id;
            users.add_user(User(name, id, meta, group_id));
            if (group_id != "none")
            {
                Group *g = groups.get_group(group_id);
                if (g)
                    g->add_user(id);
            }
        }
        else if (cmd == "deleteUser")
        {
            std::string id;
            iss >> id;
            User *u = users.get_user(id);
            if (u && u->get_group_id() != "none")
            {
                Group *g = groups.get_group(u->get_group_id());
                if (g)
                    g->remove_user(id);
            }
            users.delete_user(id);
        }
        else if (cmd == "allUsers")
        {
            std::cout << users.all_users_info();
        }
        else if (cmd == "getUser")
        {
            std::string id;
            iss >> id;
            User *u = users.get_user(id);
            if (u)
                std::cout << u->get_user_info() << "\n";
            else
                std::cout << "User not found\n";
        }
        else if (cmd == "createGroup")
        {
            std::string id, name;
            iss >> id >> name;
            groups.add_group(Group(id, name));
        }
        else if (cmd == "deleteGroup")
        {
            std::string id;
            iss >> id;
            Group *g = groups.get_group(id);
            if (g)
            {
                for (const auto &uid : g->get_user_ids())
                {
                    User *u = users.get_user(uid);
                    if (u)
                        u->set_group_id("none");
                }
            }
            groups.delete_group(id);
        }
        else if (cmd == "allGroups")
        {
            std::cout << groups.all_groups_info();
        }
        else if (cmd == "getGroup")
        {
            std::string id;
            iss >> id;
            Group *g = groups.get_group(id);
            if (g)
                std::cout << g->get_group_info() << "\n";
            else
                std::cout << "Group not found\n";
        }
        else if (cmd == "exit")
            break;
    }
    return 0;
}