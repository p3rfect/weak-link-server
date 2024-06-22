//
// Created by p3rfect on 17.06.24.
//

#include "JsonUserAdapter.h"

User JsonUserAdapter::get_user_from_json(Json::Value root) {
    User user{(root.isMember("id") ? root["id"].asInt() : 0), root["username"].asString(),
              (root.isMember("password") ? root["password"].asString() : ""),
              (root.isMember("master") ? root["master"].asBool() : false)};
    if (root.isMember("path_to_pic")) user.set_path_to_pic(root["path_to_pic"].asString());
    return user;
}

Json::Value JsonUserAdapter::get_json_from_user(User user) {
    Json::Value root;
    root["id"] = user.id;
    root["username"] = user.username;
    root["master"] = user.master;
    root["picture"]["file"] = user.get_path_to_pic();
    root["picture"]["data"] = ImageService::encode_image(user.get_path_to_pic());
    return root;
}