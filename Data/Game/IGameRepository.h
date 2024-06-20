//
// Created by p3rfect on 17.06.24.
//

#ifndef WEAKLINK_IGAMEREPOSITORY_H
#define WEAKLINK_IGAMEREPOSITORY_H

#include "../../Entities/Game.h"

class IGameRepository {
public:
    virtual void add_new_game(Game& game) = 0;
    virtual Game* get_game_by_id(int id) = 0;
    virtual void delete_game_by_id(int id) = 0;
    virtual Game* get_game_by_code(const std::string& code) = 0;
};


#endif //WEAKLINK_IGAMEREPOSITORY_H
