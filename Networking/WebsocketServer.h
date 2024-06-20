//
// Created by p3rfect on 11.06.24.
//

#ifndef WEAKLINK_WEBSOCKETSERVER_H
#define WEAKLINK_WEBSOCKETSERVER_H

#include <boost/asio.hpp>
#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <jsoncpp/json/json.h>

typedef websocketpp::server<websocketpp::config::asio> WebsocketEndpoint;
typedef websocketpp::connection_hdl ClientConnection;

class WebsocketServer {
public:
    WebsocketServer();
    void run(int port);

    size_t connections_num();

    template<typename Callback>
    void register_callback_for_connect(Callback handler);

    template<typename Callback>
    void register_callback_for_disconnect(Callback handler);

    void register_callback_for_message_type(const std::string& message_type, std::function<void(ClientConnection, const Json::Value&)> handler);

    void send_message(ClientConnection conn, const std::string& message_type, const Json::Value& arguments);

    void broadcast_message(std::vector<ClientConnection*> conns, const std::string& message_type, const Json::Value& arguments);
private:
    static Json::Value parse_json(const std::string& json);
    static std::string stringify_json(const Json::Value& value);

    void on_open(const ClientConnection& conn);
    void on_close(const ClientConnection& conn);
    void on_message(const ClientConnection& conn, const WebsocketEndpoint::message_ptr& message);

    boost::asio::io_service event_loop;
    WebsocketEndpoint endpoint;
    std::vector <ClientConnection> open_connections;
    std::mutex connection_list_mutex;

    std::vector<std::function<void(ClientConnection)>> on_connect_handlers;
    std::vector<std::function<void(ClientConnection)>> on_disconnect_handlers;
    std::map<std::string, std::vector<std::function<void(ClientConnection, const Json::Value&)>>> on_message_handlers;
};


#endif //WEAKLINK_WEBSOCKETSERVER_H
