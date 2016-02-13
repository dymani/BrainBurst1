#ifndef PHYSICS_COMPONENT_H
#define PHYSICS_COMPONENT_H

#include <SFML/Graphics.hpp>
#include <memory>
#include <LuaBridge/LuaBridge.h>
#include "BB/World/Component/IComponent.h"

namespace bb {
    class PhysicsComponent : public IComponent {
        friend class PhysicsSystem;
        friend class ControlSystem;
    public:
        PhysicsComponent() : IComponent() {
        }
    private:
        bool m_isMovable;
        int m_type;
        bool m_isOnGround;
        bool m_resistance;
        sf::Vector2f m_velocities;
        sf::FloatRect m_hitbox;
        std::shared_ptr<luabridge::LuaRef> m_onHitGround;
        std::shared_ptr<luabridge::LuaRef> m_onCollide;
    };
}

#endif