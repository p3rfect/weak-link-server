//
// Created by p3rfect on 17.06.24.
//

#ifndef WEAKLINK_GAMESERVICE_H
#define WEAKLINK_GAMESERVICE_H

#include "../Data/Game/InMemoryGameRepository.h"
#include "../Entities/Game.h"

class GameService {
public:
    GameService();
    void create_new_game(Game& game);
    Game* try_connect_player_to_game(const User& player, ClientConnection* conn, const std::string& join_code);
    Game* next_move(int game_id, bool answer_correct);
    Game* start_round(int game_id);
    Game* finish_round(int game_id);
    Game* commit_to_bank(int game_id);
    Game* eliminate_player(const std::string& username, int game_id);
    std::pair<Game*, ClientConnection*> get_strongest_player(int game_id);
private:
    IGameRepository* game_repository;
};


#endif //WEAKLINK_GAMESERVICE_H
