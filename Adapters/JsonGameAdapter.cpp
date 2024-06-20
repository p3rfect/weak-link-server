//
// Created by p3rfect on 17.06.24.
//

#include "JsonGameAdapter.h"

Game JsonGameAdapter::get_game_from_json(Json::Value root) {
    User master = JsonUserAdapter::get_user_from_json(root["master"]);
    int max_players = root["max_players"].asInt();
    int initial_round_time = root["round_time"].asInt();
    std::vector<int> levels;
    for (const auto& level : root["levels"]) levels.push_back(level.asInt());
    Game game{master, max_players, initial_round_time, levels};
    return game;
}

Json::Value JsonGameAdapter::get_json_from_game(Game game) {
    Json::Value root;
    root["master"] = JsonUserAdapter::get_json_from_user(game.master);
    for (auto& player : game.players) root["players"].append(JsonUserAdapter::get_json_from_user(player.first));
    for (auto& level : game.levels) root["levels"].append(level);
    root["id"] = game.id;
    root["code"] = game.code;
    return root;
}