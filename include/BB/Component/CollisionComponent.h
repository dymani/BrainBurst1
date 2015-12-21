#ifndef COLLISION_COMPOMENT_H
#define COLLISION_COMPONENT_H

#include <SFML/Graphics.hpp>
#include "BB/Handler/LogHandler.h"
#include "BB/Component/IComponent.h"

namespace bb {
    class CollisionComponent : public IComponent {
    public:
        static CollisionComponent* create(GameStateGame& game, luabridge::lua_State* L,            luabridge::LuaRef& luaCC);
        CollisionComponent(GameStateGame& game, int entity);
        IComponent* copy(rapidjson::Value& value);
        IComponent* copy(int entity);
        bool collide(int entity);
        sf::FloatRect getHitbox();
        bool getUpdate();
    private:
        enum Type {
            SOLID, MOVABLE, BREAKABLE
        } m_type;
        sf::Vector2i m_size;
        sf::IntRect m_hitboxI;
        sf::FloatRect m_hitboxF;
        bool m_collided;
    };
}

#endif