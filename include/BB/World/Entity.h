#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <typeindex>
#include "BB/World/Component/IComponent.h"

namespace bb {
    class GameStateGame;

    class Entity {
    public:
        Entity();
        void setId(int id);
        int getId();
        void setCoord(sf::Vector2f coord);
        sf::Vector2f getCoord();
        void addComponent(GameStateGame& game, std::type_index type, IComponent* component);
        void removeComponent(GameStateGame& game, std::type_index type);
        template<typename T>
        bool contain() {
            for(auto& component : m_components) {
                if(component = std::type_index(typeid(T)))
                    return true;
            }
            return false;
        }
    private:
        int m_id;
        sf::Vector2f m_coord;
        std::vector<std::type_index> m_components;
    };
}

#endif