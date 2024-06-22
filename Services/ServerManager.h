//
// Created by p3rfect on 17.06.24.
//

#ifndef WEAKLINK_SERVERMANAGER_H
#define WEAKLINK_SERVERMANAGER_H

#include "UserService.h"
#include "GameService.h"
#include "../Adapters/JsonUserAdapter.h"
#include "../Adapters/JsonGameAdapter.h"
#include "../Networking/WebsocketServer.h"
#include "ImageService.h"
#include <vector>
#include <string>

class ServerManager {
public:
    ServerManager();

    void start_server(int port);
private:
    void register_user_handler(ClientConnection conn, Json::Value message);
    void login_handler(ClientConnection conn, Json::Value message);
    void create_game_handler(ClientConnection conn, Json::Value message);
    void join_game_handler(ClientConnection conn, Json::Value message);
    void start_round_handler(ClientConnection conn, Json::Value message);
    void accept_answer_handler(ClientConnection conn, Json::Value message);
    void commit_to_bank_handler(ClientConnection conn, Json::Value message);
    void finish_round_handler(ClientConnection conn, Json::Value message);
    void eliminate_player_handler(ClientConnection conn, Json::Value message);
    void change_user_pic_handler(ClientConnection conn, Json::Value message);
    void get_user_pic_handler(ClientConnection conn, Json::Value message);
    void set_conn(ClientConnection conn, Json::Value message);
    void set_poll_result(ClientConnection conn, Json::Value message);
    void request_poll_result(ClientConnection conn, Json::Value message);
    void strongest_player_eliminate_handler(ClientConnection conn, Json::Value message);
    void request_eliminate(ClientConnection conn, Json::Value message);


    UserService* user_service;
    GameService* game_service;
    WebsocketServer* server;
    boost::asio::io_service main_event_loop;
    boost::asio::io_service::work work;
};


#endif //WEAKLINK_SERVERMANAGER_H
