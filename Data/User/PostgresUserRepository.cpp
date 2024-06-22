//
// Created by p3rfect on 13.06.24.
//

#include <iostream>
#include "PostgresUserRepository.h"

void PostgresUserRepository::add_user(User &user){
    std::lock_guard<std::mutex> lock(db_mutex);
    pqxx::work tx{db_connection};
    tx.exec("INSERT INTO users (username, password, is_master) VALUES ('"
                + user.username + "', '" + user.password + "', " + (user.master ? "True" : "False") + ")");
    int id = tx.query_value<int>("SELECT id FROM users WHERE username = '" + user.username + "'");
    user.id = id;
    tx.exec("INSERT INTO user_pics (user_id, path_to_pic) VALUES (" + std::to_string(user.id) + ", 'media/no_image.png')");
    tx.commit();
}

void PostgresUserRepository::update_user_picture(const User &user, const std::string& path_to_pic) {
    std::lock_guard<std::mutex> lock(db_mutex);
    pqxx::work tx{db_connection};
    tx.exec("UPDATE user_pics SET path_to_pic = '" + path_to_pic + "' WHERE user_id = " + std::to_string(user.id));
    tx.commit();
}

User PostgresUserRepository::get_user_by_username(const std::string &username) {
    std::lock_guard<std::mutex> lock(db_mutex);
    pqxx::work tx{db_connection};
    auto [id, password, is_master] = tx.query1<int, std::string, bool>
            ("SELECT id, password, is_master FROM users WHERE username = '" + username + "'");
    auto path_to_pic = tx.query_value<std::string>("SELECT path_to_pic FROM user_pics WHERE user_id = " + std::to_string(id));
    User user{id, username, password, is_master};
    std::cout << is_master << '\n';
    user.set_path_to_pic(path_to_pic);
    tx.commit();
    return user;
}

bool PostgresUserRepository::user_exist(const User &user) {
    std::lock_guard<std::mutex> lock(db_mutex);
    pqxx::work tx{db_connection};
    return tx.exec("SELECT 1 FROM users WHERE username = '" + user.username + "'").affected_rows();
}