//
// Created by p3rfect on 17.06.24.
//

#ifndef WEAKLINK_JSONUSERADAPTER_H
#define WEAKLINK_JSONUSERADAPTER_H

#include <jsoncpp/json/json.h>
#include "../Entities/User.h"
#include "../Services/ImageService.h"

class JsonUserAdapter {
public:
    static User get_user_from_json(Json::Value root);
    static Json::Value get_json_from_user(User user);
};


#endif //WEAKLINK_JSONUSERADAPTER_H
