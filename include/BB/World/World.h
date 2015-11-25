#ifndef WORLD_H
#define WORLD_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <rapidjson/document.h>
#include <string>
#include "BB/World/Field.h"
#include "BB/World/Stage.h"
#include "BB/Handler/LogHandler.h"
#include "BB/World/Entity.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Component/PlayerComponent.h"
#include "BB/Component/MovementComponent.h"

namespace bb {
    class GameStateGame;

    class World {
    public:
        World(GameStateGame& game, std::string name, luabridge::lua_State* L);
        void createField();
        void handleInput();
        void handleInput(sf::Event windowEvent);
        void update();
        void draw();
        Field* getField();
        Stage* getStage(std::string name);
        Entity* getEntity(int id);
        std::map<int, Entity*>& getEntities();
    private:
        GameStateGame& m_game;
        luabridge::lua_State* L;
        std::string m_name, m_fieldId;
        Field* m_field;
        std::map<std::string, Stage*> m_stages;
        std::map<int, Entity*> m_entities;
    };
}

#endif