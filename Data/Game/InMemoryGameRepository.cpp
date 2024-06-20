//
// Created by p3rfect on 17.06.24.
//

#include "InMemoryGameRepository.h"

InMemoryGameRepository::InMemoryGameRepository() : id_seq(0) {}

void InMemoryGameRepository::add_new_game(Game& game) {
    std::lock_guard<std::mutex> lock(games_list_mutex);
    game.id = ++id_seq;
    games.push_back(game);
}

Game* InMemoryGameRepository::get_game_by_id(int id) {
    std::lock_guard<std::mutex> lock(games_list_mutex);
    for (auto& game : games){
        if (game.id == id) return &game;
    }
    return nullptr;
}

void InMemoryGameRepository::delete_game_by_id(int id) {
    std::lock_guard<std::mutex> lock(games_list_mutex);
    games.erase(std::find_if(games.begin(), games.end(), [&id](const Game& game){ return game.id == id; }));
}

Game* InMemoryGameRepository::get_game_by_code(const std::string &code) {
    std::lock_guard<std::mutex> lock(games_list_mutex);
    for (auto& game : games){
        if (game.code == code) return &game;
    }
    throw std::logic_error("Not found");
}