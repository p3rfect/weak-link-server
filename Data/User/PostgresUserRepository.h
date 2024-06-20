//
// Created by p3rfect on 13.06.24.
//

#ifndef WEAKLINK_POSTGRESUSERREPOSITORY_H
#define WEAKLINK_POSTGRESUSERREPOSITORY_H

#include "IUserRepository.h"
#include <pqxx/pqxx>

class PostgresUserRepository : public IUserRepository {
public:
    void add_user(User &user) override;
    void update_user_picture(const User &user, const std::string& path_to_pic) override;
    User get_user_by_username(const std::string &username) override;
    bool user_exist(const User &user) override;
private:
    pqxx::connection db_connection{"host=localhost port=5432 user=weak_link password=weak_link dbname=weak_link_db"};
    std::mutex db_mutex;
};


#endif //WEAKLINK_POSTGRESUSERREPOSITORY_H
