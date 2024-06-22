//
// Created by p3rfect on 17.06.24.
//

#include "ServerManager.h"

#include <utility>
#include <fstream>

ServerManager::ServerManager() : work(main_event_loop){
    user_service = new UserService();
    game_service = new GameService();
    server = new WebsocketServer();

    server->register_callback_for_message_type("register", [&](const ClientConnection& conn, const Json::Value& message){
        main_event_loop.post([this, conn, message](){ register_user_handler(conn, message); });
    });
    server->register_callback_for_message_type("login", [&](const ClientConnection& conn, const Json::Value& message){
        main_event_loop.post([this, conn, message](){ login_handler(conn, message); });
    });
    server->register_callback_for_message_type("create_game", [&](const ClientConnection& conn, const Json::Value& message){
        main_event_loop.post([this, conn, message](){ create_game_handler(conn, message); });
    });
    server->register_callback_for_message_type("join_game", [&](const ClientConnection& conn, const Json::Value& message){
        main_event_loop.post([this, conn, message](){ join_game_handler(conn, message); });
    });
    server->register_callback_for_message_type("start_round", [&](const ClientConnection& conn, const Json::Value& message){
        main_event_loop.post([this, conn, message](){ start_round_handler(conn, message); });
    });
    server->register_callback_for_message_type("answer", [&](const ClientConnection& conn, const Json::Value& message){
        main_event_loop.post([this, conn, message](){ accept_answer_handler(conn, message); });
    });
    server->register_callback_for_message_type("commit_to_bank", [&](const ClientConnection& conn, const Json::Value& message){
        main_event_loop.post([this, conn, message](){ commit_to_bank_handler(conn, message); });
    });
    server->register_callback_for_message_type("finish_round", [&](const ClientConnection& conn, const Json::Value& message){
        main_event_loop.post([this, conn, message](){ finish_round_handler(conn, message); });
    });
    server->register_callback_for_message_type("get_players_to_eliminate", [&](const ClientConnection& conn, const Json::Value& message){
        main_event_loop.post([this, conn, message](){ eliminate_player_handler(conn, message); });
    });
    server->register_callback_for_message_type("change_user_pic", [&](const ClientConnection& conn, const Json::Value& message){
        main_event_loop.post([this, conn, message](){ change_user_pic_handler(conn, message); });
    });
    server->register_callback_for_message_type("get_user_pic", [&](const ClientConnection& conn, const Json::Value& message){
        main_event_loop.post([this, conn, message](){ get_user_pic_handler(conn, message); });
    });
    server->register_callback_for_message_type("set_conn", [&](const ClientConnection& conn, const Json::Value& message){
        main_event_loop.post([this, conn, message](){ set_conn(conn, message); });
    });
    server->register_callback_for_message_type("request_poll_result", [&](const ClientConnection& conn, const Json::Value& message){
        main_event_loop.post([this, conn, message](){ request_poll_result(conn, message); });
    });
    server->register_callback_for_message_type("reveal_poll_result", [&](const ClientConnection& conn, const Json::Value& message){
        main_event_loop.post([this, conn, message](){ set_poll_result(conn, message); });
    });
    server->register_callback_for_message_type("request_eliminate", [&](const ClientConnection& conn, const Json::Value& message){
        main_event_loop.post([this, conn, message](){ request_eliminate(conn, message); });
    });
    server->register_callback_for_message_type("eliminate", [&](const ClientConnection& conn, const Json::Value& message){
        main_event_loop.post([this, conn, message](){ strongest_player_eliminate_handler(conn, message); });
    });
}

void ServerManager::start_server(int port) {
    std::thread server_thread([&](){
        server->run(port);
    });
    main_event_loop.run();
}

void ServerManager::register_user_handler(ClientConnection conn, Json::Value message) {
    User user = JsonUserAdapter::get_user_from_json(message["user"]);
    bool success = user_service->register_user(user);
    Json::Value res;
    res["success"] = success;
    server->send_message(std::move(conn), "registration", res);
}

void ServerManager::login_handler(ClientConnection conn, Json::Value message) {
    User user = JsonUserAdapter::get_user_from_json(message["user"]);
    User actual_user = user_service->login(user);
    Json::Value res;
    if (actual_user.id == 0){
        res["success"] = false;
        server->send_message(std::move(conn), "login", res);
    }
    else{
        res["success"] = true;
        res["user"] = JsonUserAdapter::get_json_from_user(actual_user);
        server->send_message(std::move(conn), "login", res);
    }
}

void ServerManager::create_game_handler(ClientConnection conn, Json::Value message) {
    Game game = JsonGameAdapter::get_game_from_json(message["game"]);
    game.master = user_service->get_user_by_username(game.master.username);
    game_service->create_new_game(game);
    Json::Value game_json = JsonGameAdapter::get_json_from_game(game);
    server->send_message(std::move(conn), "create_game", game_json);
}

void ServerManager::join_game_handler(ClientConnection conn, Json::Value message) {
    User user = user_service->get_user_by_username(JsonUserAdapter::get_user_from_json(message["user"]).username);
    Game* game = game_service->try_connect_player_to_game(user, conn, message["code"].asString());
    Json::Value res;
    if (game == nullptr){
        res["success"] = false;
        server->send_message(conn, "join_game", res);
        return;
    }
    std::cout << "!!!!!" << game->players.size() << '\n';
    res["success"] = true;
    res["game"] = JsonGameAdapter::get_json_from_game(*game);
    Json::Value player_connected_message;
    player_connected_message["user"] = JsonUserAdapter::get_json_from_user(user);
    server->send_message(conn, "join_game", res);
    for (auto& player_conn : game->get_players_connections()){
        if (player_conn.lock() != conn.lock()) server->send_message(player_conn, "player_join", player_connected_message);
    }
    server->send_message(game->master_conn, "player_join", player_connected_message);
}

void ServerManager::start_round_handler(ClientConnection conn, Json::Value message) {
    int game_id = message["game_id"].asInt();
    Game* game = game_service->start_round(game_id);
    Json::Value res;
    res["round_time"] = game->current_round_seconds;
    server->broadcast_message(game->get_players_connections(), "round_started", res);
    server->send_message(std::move(conn), "round_started", res);
}

void ServerManager::accept_answer_handler(ClientConnection conn, Json::Value message) {
    int game_id = message["game_id"].asInt();
    bool answer = message["answer"].asBool();
    Game* game = game_service->next_move(game_id, answer);
    Json::Value res;
    res["answer"] = answer;
    server->broadcast_message(game->get_players_connections(), "answer", res);
    server->send_message(std::move(conn), "answer", res);
}

void ServerManager::commit_to_bank_handler(ClientConnection conn, Json::Value message) {
    int game_id = message["game_id"].asInt();
    Game* game = game_service->commit_to_bank(game_id);
    server->broadcast_message(game->get_players_connections(), "commit_to_bank", Json::Value{});
    server->send_message(std::move(conn), "commit_to_bank", Json::Value{});
}

void ServerManager::finish_round_handler(ClientConnection conn, Json::Value message) {
    int game_id = message["game_id"].asInt();
    Game* game = game_service->finish_round(game_id);
    server->broadcast_message(game->get_players_connections(), "finish_round", Json::Value{});
    server->send_message(std::move(conn), "finish_round", Json::Value{});
}

void ServerManager::eliminate_player_handler(ClientConnection conn, Json::Value message) {
    int game_id = message["game_id"].asInt();
    std::vector<std::string> usernames_to_eliminate = game_service->get_usernames_to_eliminate(game_id);
    if (usernames_to_eliminate.size() == 1){
        Game* game = game_service->eliminate_player(game_id, usernames_to_eliminate[0]);
        Json::Value res;
        res["username"] = usernames_to_eliminate[0];
        server->broadcast_message(game->get_players_connections(), "player_eliminated", res);
        server->send_message(game->master_conn, "player_eliminated", res);
    }
    else{
        Json::Value res;
        for (auto username : usernames_to_eliminate) res["players"].append(username);
        auto [game, strongest_player_conn] = game_service->get_strongest_player(game_id);
        for (auto conn : game->get_players_connections()){
            if (conn.lock() != strongest_player_conn.lock()){
                server->send_message(conn, "strongest_player_eliminates", res);
            }
        }
        server->send_message(game->master_conn, "strongest_player_eliminates", res);
        server->send_message(strongest_player_conn, "choose_eliminate", res);
    }
}

void ServerManager::strongest_player_eliminate_handler(ClientConnection conn, Json::Value message) {
    int game_id = message["game_id"].asInt();
    std::string username = message["username"].asString();
    Game* game = game_service->eliminate_player(game_id, username);
    Json::Value res;
    res["username"] = username;
    server->broadcast_message(game->get_players_connections(), "player_eliminated", res);
    server->send_message(game->master_conn, "player_eliminated", res);
}

void ServerManager::request_eliminate(ClientConnection conn, Json::Value message) {
    int game_id = message["game_id"].asInt();
    auto [game, strongest_player_conn] = game_service->get_strongest_player(game_id);
    Json::Value res;
    server->send_message(strongest_player_conn, "request_eliminate", res);
}

void ServerManager::change_user_pic_handler(ClientConnection conn, Json::Value message) {
    std::string username = message["username"].asString();
    std::string picture = message["picture"].asString();
    std::string filename = "media/";
    std::uniform_int_distribution<int> dist(0, 35);
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = 0; i < 20; i++){
        int curr = dist(gen);
        filename += static_cast<char>(curr < 10 ? '0' + curr : 'a' + curr - 10);
    }
    filename += ".png";
    Json::Value res;
    if (ImageService::save_image(filename, picture)){
        user_service->update_user_pic(User(username, ""), filename);
        res["success"] = true;
    }
    else{
        res["success"] = false;
    }
    server->send_message(std::move(conn), "change_picture", res);
}

void ServerManager::get_user_pic_handler(ClientConnection conn, Json::Value message) {
    std::string username = message["username"].asString();
    Json::Value res;
    res["picture"] = ImageService::encode_image(user_service->get_path_to_pic(username));
    server->send_message(std::move(conn), "get_user_pic", res);
}

void ServerManager::set_conn(ClientConnection conn, Json::Value message) {
    std::string username = message["username"].asString();
    int game_id = message["game_id"].asInt();
    game_service->set_conn(std::move(conn), game_id, username);
}

void ServerManager::set_poll_result(ClientConnection conn, Json::Value message) {
    int game_id = message["game_id"].asInt();
    std::string username = message["username"].asString();
    std::string result = message["result"].asString();
    Game* game = game_service->set_poll_result(game_id, username, result);
    Json::Value res;
    res["username"] = username;
    res["result"] = result;
    server->broadcast_message(game->get_players_connections(), "reveal_poll_result", res);
    server->send_message(game->master_conn, "reveal_poll_result", res);
}

void ServerManager::request_poll_result(ClientConnection conn, Json::Value message) {
    int game_id = message["game_id"].asInt();
    std::string username = message["username"].asString();
    Json::Value res;
    server->send_message(game_service->get_player_conn(game_id, username), "request_poll_result", res);
}