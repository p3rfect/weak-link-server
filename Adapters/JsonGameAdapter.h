//
// Created by p3rfect on 17.06.24.
//

#ifndef WEAKLINK_JSONGAMEADAPTER_H
#define WEAKLINK_JSONGAMEADAPTER_H

#include <jsoncpp/json/json.h>
#include "../Entities/Game.h"
#include "JsonUserAdapter.h"

class JsonGameAdapter {
public:
    static Game get_game_from_json(Json::Value root);
    static Json::Value get_json_from_game(Game game);
};


#endif //WEAKLINK_JSONGAMEADAPTER_H
