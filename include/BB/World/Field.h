#ifndef FIELD_H
#define FIELD_H

#include <LuaBridge\LuaBridge.h>
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
        void draw();
    private:
        GameStateGame& m_game;
        std::string m_id;
        luabridge::lua_State* L;
        sf::RenderStates m_states;
        sf::VertexArray m_vertices;
        std::string m_tileSet, m_objectTexture;
        int m_tiles[100];
        std::map<std::string, Entity*> m_objects;
    };
}

#endif