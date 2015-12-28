#ifndef MOVEMENT_COMPONENT_H
#define MOVEMENT_COMPONENT_H

#include "BB/World/Component/IComponent.h"
#include <SFML/Graphics.hpp>

namespace bb {
    class MovementComponent : public IComponent {
        friend class PhysicsSystem;
        friend class ControlSystem;
    public:
        MovementComponent() : IComponent() {
        }
    private:
        bool m_isOnGround;
        sf::Vector2f m_velocities;
    };
}

#endif