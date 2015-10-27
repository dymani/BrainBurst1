#ifndef WORLD_H
#define WORLD_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <string>
#include "BB/World/Field.h"
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
        Entity* getEntity(int id);
        std::vector<Entity*>& getEntities();
    private:
        GameStateGame& m_game;
        luabridge::lua_State* L;
        std::string m_name, m_fieldId;
        Field* m_field;
        std::vector<Entity*> m_entities;
    };
}

#endif