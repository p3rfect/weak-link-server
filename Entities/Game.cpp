//
// Created by p3rfect on 14.06.24.
//

#include "Game.h"

#include <utility>

Game::Game(User game_master, int players_number, int initial_round_time,
           const std::vector<int>& custom_levels) : master(std::move(game_master)), max_players(players_number),
                                                    current_round_seconds(initial_round_time), round_number(0),
                                                    bank(0), round_finished(false), game_started(false), current_level(0),
                                                    game_finished(false), current_player(0), id(0) {
    if (!custom_levels.empty()) levels = custom_levels;
    std::uniform_int_distribution<int> dist(0, 25);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::string join_code;
    for (int i = 0; i < 5; i++){
        join_code += static_cast<char>('A' + dist(gen));
    }
    code = join_code;
}

void Game::start_round() {
    if (game_finished) throw std::logic_error("Game is finished");
    if (!round_finished && game_started) throw std::logic_error("Round is not finished");
    current_player = 0;
    std::fill(player_stats.begin(), player_stats.end(), std::make_pair(0, 0));
    round_number++;
    current_level = 0;
    round_finished = false;
    game_started = true;
}

void Game::end_round() {
    if (!game_started) throw std::logic_error("Game is not started");
    if (round_finished) throw std::logic_error("Round is already finished");
    current_round_seconds -= 10;
    round_finished = true;
    if (round_number == players.size()) game_finished = true;
}

int Game::get_current_level() {
    return current_level;
}

void Game::next_move(bool answer_correct) {
    if (answer_correct) current_level++, player_stats[current_player].first++;
    else current_level = 0;
    current_player = (current_player + 1) % eliminated.size();
    while (eliminated[(current_player) % eliminated.size()]) current_player++;
}

void Game::commit_to_bank() {
    bank += levels[current_level];
    current_level = 0;
}

void Game::add_player(const User& user, ClientConnection conn) {
    if (players.size() == max_players) throw std::overflow_error("Max players number is reached");
    if (game_started) throw std::logic_error("Game has been already started");
    players.emplace_back(user, conn);
    eliminated.push_back(false);
    player_stats.emplace_back(0, 0);
    poll_results.emplace_back("");
}

ClientConnection Game::get_strongest_player() {
    int index = std::distance(player_stats.begin(), std::max_element(player_stats.begin(), player_stats.end()));
    return players[index].second;
}

std::vector<ClientConnection> Game::get_players_connections() {
    std::vector<ClientConnection> res;
    for (auto& player : players){
        res.push_back(player.second);
    }
    return res;
}

void Game::set_poll_result(std::string username, std::string result) {
    for (int i = 0; i < players.size(); i++){
        if (players[i].first.username == username) poll_results[i] = result;
    }
}

ClientConnection Game::get_player_conn(std::string username) {
    for (auto player : players){
        if (player.first.username == username) return player.second;
    }
}

void Game::get_players_to_eliminate() {
    std::map<std::string, int> vote_count;
    for (auto vote : poll_results){
        vote_count[vote]++;
    }
    int max_votes = 0;
    std::vector<std::string> usernames_to_eliminate;
    for (auto [username, vote_num] : vote_count){
        if (vote_num > max_votes){
            usernames_to_eliminate.clear();
            usernames_to_eliminate.push_back(username);
            max_votes = vote_num;
        }
        else if (vote_num == max_votes) usernames_to_eliminate.push_back(username);
    }
    players_to_eliminate = usernames_to_eliminate;
}

void Game::eliminate_player(std::string username) {
    for (int i = 0; i < players.size(); i++){
        if (players[i].first.username == username){
            eliminated[i] = true;
            break;
        }
    }
}