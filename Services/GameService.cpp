//
// Created by p3rfect on 17.06.24.
//

#include "GameService.h"

#include <utility>

GameService::GameService() {
    game_repository = new InMemoryGameRepository();
}

void GameService::create_new_game(Game& game) {
    game_repository->add_new_game(game);
}

Game* GameService::try_connect_player_to_game(const User& player, ClientConnection conn, const std::string& join_code) {
    try{
        Game* game = game_repository->get_game_by_code(join_code);
        game->add_player(player, std::move(conn));
        return game;
    }
    catch (...){
        return nullptr;
    }
}

Game* GameService::next_move(int game_id, bool answer_correct){
    Game* game = game_repository->get_game_by_id(game_id);
    game->next_move(answer_correct);
    return game;
}

Game* GameService::start_round(int game_id) {
    Game* game = game_repository->get_game_by_id(game_id);
    game->start_round();
    return game;
}

Game* GameService::finish_round(int game_id) {
    Game* game = game_repository->get_game_by_id(game_id);
    game->end_round();
    return game;
}

Game* GameService::commit_to_bank(int game_id) {
    Game* game = game_repository->get_game_by_id(game_id);
    game->commit_to_bank();
    return game;
}

std::pair<Game*, ClientConnection> GameService::get_strongest_player(int game_id) {
    Game* game = game_repository->get_game_by_id(game_id);
    return {game, game->get_strongest_player()};
}

void GameService::set_conn(ClientConnection conn, int game_id, const std::string& username) {
    Game* game = game_repository->get_game_by_id(game_id);
    if (game->master.username == username) game->master_conn = std::move(conn);
    else{
        for (auto &player : game->players){
            if (player.first.username == username) player.second = std::move(conn);
            break;
        }
    }
}

Game* GameService::set_poll_result(int game_id, std::string username, std::string result) {
    Game* game = game_repository->get_game_by_id(game_id);
    game->set_poll_result(username, result);
    return game;
}

ClientConnection GameService::get_player_conn(int game_id, std::string username) {
    Game* game = game_repository->get_game_by_id(game_id);
    return game->get_player_conn(username);
}

std::vector<std::string> GameService::get_usernames_to_eliminate(int game_id) {
    Game* game = game_repository->get_game_by_id(game_id);
    game->get_players_to_eliminate();
    std::vector<std::string> usernames = game->players_to_eliminate;
    return usernames;
}

Game* GameService::eliminate_player(int game_id, std::string username) {
    Game* game = game_repository->get_game_by_id(game_id);
    game->eliminate_player(username);
    return game;
}