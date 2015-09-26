#include "BB/Entity.h"
#include "BB/Component/IComponent.h"

namespace bb {
    Entity::Entity() {
    }

    Entity::Entity(const Entity& rhs) {
        m_components = rhs.m_components;
        m_coord = rhs.m_coord;
    }

    Entity::~Entity() {
    }

    Entity* Entity::addComponent(std::type_index type, IComponent* component) {
        m_components[type] = component;
        return this;
    }

    Entity* Entity::setCoord(sf::Vector2f coord) {
        m_coord = coord;
        return this;
    }

    sf::Vector2f Entity::getCoord() {
        return m_coord;
    }
}