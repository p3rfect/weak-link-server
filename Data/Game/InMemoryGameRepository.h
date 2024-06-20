//
// Created by p3rfect on 17.06.24.
//

#ifndef WEAKLINK_INMEMORYGAMEREPOSITORY_H
#define WEAKLINK_INMEMORYGAMEREPOSITORY_H

#include "IGameRepository.h"

class InMemoryGameRepository : public IGameRepository {
public:
    void add_new_game(Game& game) override;
    Game* get_game_by_id(int id) override;
    void delete_game_by_id(int id) override;
    Game* get_game_by_code(const std::string &code) override;
    InMemoryGameRepository();
private:
    std::vector<Game> games;
    std::mutex games_list_mutex;
    int id_seq;
};


#endif //WEAKLINK_INMEMORYGAMEREPOSITORY_H
