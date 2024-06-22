//
// Created by p3rfect on 17.06.24.
//

#ifndef WEAKLINK_GAMESERVICE_H
#define WEAKLINK_GAMESERVICE_H

#include "../Data/Game/InMemoryGameRepository.h"
#include "../Entities/Game.h"
#include <vector>
#include <string>

class GameService {
public:
    GameService();
    void create_new_game(Game& game);
    Game* try_connect_player_to_game(const User& player, ClientConnection conn, const std::string& join_code);
    Game* next_move(int game_id, bool answer_correct);
    Game* start_round(int game_id);
    Game* finish_round(int game_id);
    Game* commit_to_bank(int game_id);
    std::pair<Game*, ClientConnection> get_strongest_player(int game_id);
    void set_conn(ClientConnection conn, int game_id, const std::string& username);
    Game* set_poll_result(int game_id, std::string username, std::string result);
    ClientConnection get_player_conn(int game_id, std::string username);
    std::vector<std::string> get_usernames_to_eliminate(int game_id);
    Game* eliminate_player(int game_id, std::string username);
private:
    IGameRepository* game_repository;
};


#endif //WEAKLINK_GAMESERVICE_H
