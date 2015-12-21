#ifndef FIELD_H
#define FIELD_H

#include <LuaBridge/LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include "BB/Handler/LogHandler.h"
#include "BB/World/Entity.h"

namespace bb {
    class GameStateGame;

    class Field {
    public:
        Field(GameStateGame& game, luabridge::lua_State* L, std::string id);
        void load(std::string worldName);
        void draw();
        int getSize();
    private:
        GameStateGame& m_game;
        std::string m_id;
        luabridge::lua_State* L;
        sf::RenderStates m_states;
        sf::VertexArray m_vertices;
        std::string m_tileSet;
        std::vector<int> m_tiles;
        std::map<std::string, Entity*> m_objectsTemplate;
        std::map<std::string, Entity*> m_entitiesTemplate;
        std::vector<int> m_entities;
        std::vector<int> m_objects;
        sf::Sprite m_background;
    };
}

#endif