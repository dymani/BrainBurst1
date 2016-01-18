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
        Entity(GameStateGame& game);
        ~Entity();
        void setId(int id);
        int getId();
        void setCoord(sf::Vector2f coord);
        sf::Vector2f getCoord();
        void addComponent(std::type_index type, IComponent* component);
        void removeComponent(std::type_index type);
        template<typename T>
        bool contains() {
            for(auto& component : m_components) {
                if(component = std::type_index(typeid(T)))
                    return true;
            }
            return false;
        }
    private:
        GameStateGame& m_game;
        int m_id;
        sf::Vector2f m_coord;
        std::vector<std::type_index> m_components;
    };
}

#endif