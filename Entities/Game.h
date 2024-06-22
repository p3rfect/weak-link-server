//
// Created by p3rfect on 14.06.24.
//

#ifndef WEAKLINK_GAME_H
#define WEAKLINK_GAME_H

#include <vector>
#include <string>
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
    std::vector<std::pair<User, ClientConnection>> players;
    std::vector<bool> eliminated;
    std::vector<std::pair<int, int>> player_stats;
    User master;
    ClientConnection master_conn;
    std::vector<std::string> poll_results;
    std::vector<std::string> players_to_eliminate;

    Game(User game_master, int players_number, int initial_round_time,
         const std::vector<int>& custom_levels = {});

    void start_round();
    void end_round();
    int get_current_level();
    void next_move(bool answer_correct);
    void commit_to_bank();
    void add_player(const User& user, ClientConnection conn);
    ClientConnection get_strongest_player();
    std::vector<ClientConnection> get_players_connections();
    void set_poll_result(std::string username, std::string result);
    ClientConnection get_player_conn(std::string username);
    void get_players_to_eliminate();
    void eliminate_player(std::string username);
private:
    int current_level;
};


#endif //WEAKLINK_GAME_H
