#ifndef COLLISION_COMPOMENT_H
#define COLLISION_COMPONENT_H

#include <SFML/Graphics.hpp>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include "BB/Handler/LogHandler.h"
#include "BB/Component/IComponent.h"

namespace bb {
    class CollisionComponent : public IComponent {
    public:
        static CollisionComponent* create(GameStateGame& game, int entity, luabridge::lua_State* L,
            luabridge::LuaRef& luaCC);
        CollisionComponent(GameStateGame& game, int entity);
        IComponent* copy(int entity);
        bool collide(int entity);
        sf::FloatRect getHitbox();
    private:
        enum Type {
            SOLID, MOVABLE, BREAKABLE
        } m_type;
        sf::Vector2i m_size;
        sf::IntRect m_hitboxI;
        sf::FloatRect m_hitboxF;
    };
}

#endif