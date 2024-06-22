//
// Created by p3rfect on 17.06.24.
//

#ifndef WEAKLINK_USERSERVICE_H
#define WEAKLINK_USERSERVICE_H

#include "../Data/User/IUserRepository.h"
#include "../Data/User/PostgresUserRepository.h"

class UserService {
public:
    UserService();

    bool register_user(User user);
    User login(User& user);
    void update_user_pic(const User& user, const std::string& path_to_pic);
    std::string get_path_to_pic(const std::string& username);
    User get_user_by_username(const std::string &username);
private:
    IUserRepository* user_repository;
};


#endif //WEAKLINK_USERSERVICE_H
