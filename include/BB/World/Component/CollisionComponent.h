#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <LuaBridge/LuaBridge.h>
#include "BB/World/Component/IComponent.h"

namespace bb {
    class CollisionComponent : public IComponent {
        friend class PhysicsSystem;
    public:
        CollisionComponent() : IComponent() {
        }
    private:
        sf::FloatRect m_hitbox;
        std::shared_ptr<luabridge::LuaRef> m_collideFunc;
    };
}

#endif