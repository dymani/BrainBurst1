#include "BB/World/Entity.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    Entity::Entity(GameStateGame& game) : m_game(game) {
        m_components.clear();
    }

    Entity::~Entity() {
        while(!m_components.empty()) {
            removeComponent(*m_components.begin());
        }
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

    void Entity::addComponent(std::type_index type, IComponent* component) {
        auto& list = m_game.getWorld().getField()->getComponentList(type)->m_list;
        list[m_id] = std::unique_ptr<IComponent>(component);
        m_components.push_back(std::type_index(typeid(*component)));
    }

    void Entity::removeComponent(std::type_index type) {
        auto* list = m_game.getWorld().getField()->getComponentList(type);
        if(list)list->m_list.erase(m_id);
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