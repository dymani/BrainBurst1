#include "BB/Entity.h"
#include "BB/Component/IComponent.h"

namespace bb {
    Entity::Entity() {
    }

    Entity::~Entity() {
        for(auto& c : m_components) {
            delete c.second;
        }
    }

    Entity* Entity::addComponent(std::type_index type, IComponent* component) {
        m_components[type] = component;
        return this;
    }

    Entity* Entity::setZ(float z) {
        m_z = z;
        return this;
    }

    float Entity::getZ() {
        return m_z;
    }

    Entity* Entity::setCoord(sf::Vector2i coord) {
        m_coord = coord;
        return this;
    }

    sf::Vector2i Entity::getCoord() {
        return m_coord;
    }
}