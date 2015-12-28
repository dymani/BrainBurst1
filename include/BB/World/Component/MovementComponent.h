#ifndef MOVEMENT_COMPONENT_H
#define MOVEMENT_COMPONENT_H

#include "BB/World/Component/IComponent.h"
#include <SFML/Graphics.hpp>

namespace bb {
    class MovementComponent : public IComponent {
        friend class PhysicsSystem;
    public:
        MovementComponent() : IComponent() {
        }
    private:
        sf::Vector2f m_velocities;
    };
}

#endif