//
// Created by p3rfect on 17.06.24.
//

#include "UserService.h"

UserService::UserService() {
    user_repository = new PostgresUserRepository();
}

bool UserService::register_user(User user){
    if (user_repository->user_exist(user)) return false;
    user_repository->add_user(user);
    return true;
}

bool UserService::login(User &user) {
    if (!user_repository->user_exist(user)) return false;
    User actual_user = user_repository->get_user_by_username(user.username);
    if (user.password == actual_user.password){
        user = actual_user;
        return true;
    }
    else{
        return false;
    }
}

void UserService::update_user_pic(const User& user, const std::string& path_to_pic) {
    user_repository->update_user_picture(user_repository->get_user_by_username(user.username), path_to_pic);
}

std::string UserService::get_path_to_pic(const std::string &username) {
    return user_repository->get_user_by_username(username).get_path_to_pic();
}