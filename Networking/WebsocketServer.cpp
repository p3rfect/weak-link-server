//
// Created by p3rfect on 11.06.24.
//

#include "WebsocketServer.h"

#include <utility>

#define MESSAGE_TYPE_FIELD "__MESSAGE__"

Json::Value WebsocketServer::parse_json(const std::string &json) {
    Json::Value root;
    Json::Reader reader;
    reader.parse(json, root);
    return root;
}

std::string WebsocketServer::stringify_json(const Json::Value& val)
{
    Json::StreamWriterBuilder wbuilder;
    wbuilder["commentStyle"] = "None";
    wbuilder["indentation"] = "";

    return Json::writeString(wbuilder, val);
}

WebsocketServer::WebsocketServer() {
    this->endpoint.set_open_handler([this](auto && PH1) { on_open(std::forward<decltype(PH1)>(PH1)); });
    this->endpoint.set_close_handler([this](auto && PH1) { on_close(std::forward<decltype(PH1)>(PH1)); });
    this->endpoint.set_message_handler([this](auto && PH1, auto && PH2) { on_message(std::forward<decltype(PH1)>(PH1), std::forward<decltype(PH2)>(PH2)); });
    this->endpoint.init_asio(&(this->event_loop));
}

void WebsocketServer::run(int port){
    this->endpoint.listen(port);
    this->endpoint.start_accept();
    this->endpoint.run();
}

size_t WebsocketServer::connections_num() {
    std::lock_guard<std::mutex> lock(this->connection_list_mutex);

    return this->open_connections.size();
}

template<typename Callback>
void WebsocketServer::register_callback_for_connect(Callback handler) {
    this->event_loop.post([this, handler](){
        this->on_connect_handlers.push_back(handler);
    });
}

template<typename Callback>
void WebsocketServer::register_callback_for_disconnect(Callback handler) {
    this->event_loop.post([this, handler](){
        this->on_disconnect_handlers.push_back(handler);
    });
}

void WebsocketServer::register_callback_for_message_type(const std::string &message_type, std::function<void(ClientConnection, const Json::Value&)> handler) {
    this->event_loop.post([this, message_type, handler](){
        this->on_message_handlers[message_type].push_back(handler);
    });
}

void WebsocketServer::on_open(const ClientConnection& conn){
    {
        std::lock_guard<std::mutex> lock(this->connection_list_mutex);
        this->open_connections.push_back(conn);
    }

    for (auto& handler : this->on_connect_handlers){
        handler(conn);
    }
}

void WebsocketServer::send_message(ClientConnection conn, const std::string &message_type,
                                  const Json::Value &arguments) {
    Json::Value message_data = arguments;
    message_data[MESSAGE_TYPE_FIELD] = message_type;

    this->endpoint.send(std::move(conn), WebsocketServer::stringify_json(message_data), websocketpp::frame::opcode::text);
}

void WebsocketServer::broadcast_message(std::vector<ClientConnection*> conns, const std::string &message_type,
                                        const Json::Value &arguments) {
    Json::Value message_data = arguments;
    message_data[MESSAGE_TYPE_FIELD] = message_type;

    for (auto& conn : conns){
        this->endpoint.send(std::move(*conn), WebsocketServer::stringify_json(message_data), websocketpp::frame::opcode::text);
    }
}

void WebsocketServer::on_message(const ClientConnection& conn, const WebsocketEndpoint::message_ptr& message) {
    Json::Value message_object = WebsocketServer::parse_json(message->get_payload());
    if (!message_object.isNull() && message_object.isMember(MESSAGE_TYPE_FIELD)){
        std::string message_type = message_object[MESSAGE_TYPE_FIELD].asString();
        message_object.removeMember(MESSAGE_TYPE_FIELD);

        auto& handlers = this->on_message_handlers[message_type];
        for (auto handler : handlers){
            handler(conn, message_object);
        }
    }
}

void WebsocketServer::on_close(const ClientConnection& conn){
    {
        std::lock_guard<std::mutex> lock(this->connection_list_mutex);

        auto conn_value = conn.lock();
        auto new_end = std::remove_if(this->open_connections.begin(), this->open_connections.end(), [&conn_value](const ClientConnection& element){
            if (element.expired()) return true;

            auto element_value = element.lock();
            return element_value == conn_value;
        });

        this->open_connections.resize(std::distance(open_connections.begin(), new_end));
    }

    for (auto& handler : on_disconnect_handlers){
        handler(conn);
    }
}