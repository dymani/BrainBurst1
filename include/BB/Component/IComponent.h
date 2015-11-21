#ifndef I_COMPONENT_H
#define I_COMPONENT_H

#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <rapidjson/document.h>

namespace bb {
    class GameStateGame;

    class IComponent {
    public:
        virtual ~IComponent() {
        };
        virtual IComponent* copy(rapidjson::Value& value) = 0;
        virtual IComponent* copy(int entity) = 0;
    protected:
        int const m_entity;
        GameStateGame& m_game;
        const bool m_isEntity;
        luabridge::lua_State* L;
        IComponent(GameStateGame& game, int entity = -1) :m_game(game), m_entity(entity),
            m_isEntity(entity != -1) {
        }
    };
}

#endif