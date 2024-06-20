//
// Created by p3rfect on 13.06.24.
//

#include "User.h"

#include <utility>

User::User(std::string  username, std::string  password) :
    username(std::move(username)), password(std::move(password)), master(false), id(0)  {}

User::User(int id, std::string username, std::string password, bool is_master) :
    id(id), username(std::move(username)), password(std::move(password)), master(is_master) {}

void User::set_path_to_pic(std::string path) {
    path_to_pic = std::move(path);
}

std::string User::get_path_to_pic() {
    return path_to_pic;
}