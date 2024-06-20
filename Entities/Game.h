//
// Created by p3rfect on 14.06.24.
//

#ifndef WEAKLINK_GAME_H
#define WEAKLINK_GAME_H

#include <vector>
#include <websocketpp/server.hpp>
#include <random>
#include "User.h"

typedef websocketpp::connection_hdl ClientConnection;

class Game {
public:
    int id, round_number, bank, max_players, current_round_seconds, current_player;
    std::vector<int> levels = {1000, 2000, 5000, 10000, 20000, 30000, 40000, 50000};
    bool round_finished, game_started, game_finished;
    std::string code;
    std::vector<std::pair<User, ClientConnection*>> players;
    std::vector<bool> eliminated;
    std::vector<std::pair<int, int>> player_stats;
    User master;
    ClientConnection* master_conn;

    Game(User game_master, int players_number, int initial_round_time,
         const std::vector<int>& custom_levels = {});

    void start_round();
    void end_round();
    int get_current_level();
    void next_move(bool answer_correct);
    void commit_to_bank();
    void add_player(const User& user, ClientConnection* conn);
    void eliminate_player(const std::string& username);
    ClientConnection* get_strongest_player();
    std::vector<ClientConnection*> get_players_connections();
private:
    int current_level;
};


#endif //WEAKLINK_GAME_H
