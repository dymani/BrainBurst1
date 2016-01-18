#ifndef FIELD_H
#define FIELD_H

#include "BB/World/Entity.h"

namespace bb {
    class GameStateGame;

    class Field {
    public:
        Field(GameStateGame& game, std::string worldName, std::string id);
        void load();
        void update();
        void draw(const double dt);
        int getSize();
        Entity* getEntity(int id);
        void addDeleteEntity(int id);
        void deleteEntities();
        template<typename T>
        void addDeleteComponent(int id) {
            m_deletingComponents.push_back({std::type_index(typeid(T)), id});
        }
        void deleteComponents();
        template<typename T>
        T* getComponent(int id) {
            auto& list = *m_componentLists[std::type_index(typeid(T))];
            return dynamic_cast<T*>(list[id]);
        }
        std::map<int, IComponent*>* getComponentList(std::type_index type);
        template<typename T>
        std::map<int, IComponent*>* getComponentList() {
            return m_componentLists[std::type_index(typeid(T))];
        }
    private:
        GameStateGame& m_game;
        std::string m_worldName, m_id;
        sf::RenderStates m_states;
        sf::VertexArray m_vertices;
        std::string m_tileSet;
        std::vector<int> m_tiles;
        sf::Sprite m_background;
        std::map<int, Entity*> m_entities;
        std::map<std::type_index, std::map<int, IComponent*>*> m_componentLists;
        int m_playerId;
        std::vector<int> m_deletingEntities;
        std::vector<std::pair<std::type_index, int>> m_deletingComponents;
    };
}

#endif