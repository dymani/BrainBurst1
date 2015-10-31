#ifndef PLAYER_COMPONENT_H
#define PLAYER_COMPONENT_H

#include "BB/Component/IComponent.h"
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include "BB/Handler/LogHandler.h"
#include "BB/Handler/ResourceHandler.h"

namespace bb {
    class PlayerComponent: public IComponent {
    public:
        static PlayerComponent* create(GameStateGame& game, luabridge::lua_State* L, luabridge::LuaRef& luaPC);
        PlayerComponent(GameStateGame& game, int entity);
        IComponent* copy(int entity);
        void handleInput();
    private:
        enum State {
            IDLE, JUMPING
        } m_state;
    };
}

#endif