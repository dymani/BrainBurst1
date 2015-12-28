#include "BB/World/Entity.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    Entity::Entity() {

    }

    void Entity::setId(int id) {
        m_id = id;
    }

    int Entity::getId() {
        return m_id;
    }

    void Entity::setCoord(sf::Vector2f coord) {
        m_coord = coord;
    }

    sf::Vector2f Entity::getCoord() {
        return m_coord;
    }

    void Entity::addComponent(GameStateGame& game, std::type_index type, IComponent* component) {
        auto& list = *game.getWorld().getField()->getComponentList(type);
        list[m_id] = component;
        m_components.push_back(std::type_index(typeid(*component)));
    }

    void Entity::removeComponent(GameStateGame& game, std::type_index type) {
        auto& list = *game.getWorld().getField()->getComponentList(type);
        list.erase(m_id);
        auto& it = m_components.begin();
        while(it != m_components.end()) {
            if(*it == type) {
                m_components.erase(it);
                break;
            }
            it++;
        }
    }
}