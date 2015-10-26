#ifndef MOVEMENT_COMPONENT_H
#define MOVEMENT_COMPONENT_H

#include "BB/Component/IComponent.h"
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <SFML/Graphics.hpp>
#include "BB/Handler/LogHandler.h"

namespace bb {
    class MovementComponent: public IComponent {
    public:
        static MovementComponent* create(GameStateGame& game, int entity, luabridge::lua_State* L,
            luabridge::LuaRef& luaMC);
        MovementComponent(GameStateGame& game, int entity);
        IComponent* copy(int entity);
        void update(bool move);
        void setVelocity(sf::Vector2f velocity);
        sf::Vector2f getNewCoord();
        sf::Vector2f getVelocity();
    private:
        bool m_init = false;
        sf::Vector2f m_newCoord;
        sf::Vector2f m_velocity;
    };
}

#endif