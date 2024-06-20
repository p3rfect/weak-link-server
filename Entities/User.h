//
// Created by p3rfect on 13.06.24.
//

#ifndef WEAKLINK_USER_H
#define WEAKLINK_USER_H

#include <string>

class User {
public:
    int id;
    std::string username;
    std::string password;
    bool master;

    User(std::string  username, std::string  password);
    User(int id, std::string username, std::string password, bool is_master);

    void set_path_to_pic(std::string path);
    std::string get_path_to_pic();
private:
    std::string path_to_pic;
};


#endif //WEAKLINK_USER_H
