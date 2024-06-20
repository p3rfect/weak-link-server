//
// Created by p3rfect on 13.06.24.
//

#ifndef WEAKLINK_IUSERREPOSITORY_H
#define WEAKLINK_IUSERREPOSITORY_H

#include "../../Entities/User.h"

class IUserRepository {
public:
    virtual void add_user(User& user) = 0;
    virtual void update_user_picture(const User& user, const std::string& path_to_pic) = 0;
    virtual User get_user_by_username(const std::string& username) = 0;
    virtual bool user_exist(const User& user) = 0;
};


#endif //WEAKLINK_IUSERREPOSITORY_H
