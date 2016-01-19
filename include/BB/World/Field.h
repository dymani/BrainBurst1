#ifndef FIELD_H
#define FIELD_H

#include <memory>
#include "BB/World/Entity.h"
#include "BB/World/System/ISystem.h"

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
            if(m_componentLists.find(std::type_index(typeid(T))) == m_componentLists.end()) return nullptr;
            auto& list = *m_componentLists[std::type_index(typeid(T))].get();
            if(list.m_list.find(id) == list.m_list.end()) return nullptr;
            return dynamic_cast<T*>(list.m_list[id].get());
        }
        CList* getComponentList(std::type_index type);
        template<typename T>
        CList* getComponentList() {
            if(m_componentLists.find(std::type_index(typeid(T))) == m_componentLists.end()) return nullptr;
            return m_componentLists[std::type_index(typeid(T))].get();
        }
    private:
        GameStateGame& m_game;
        std::string m_worldName, m_id;
        sf::RenderStates m_states;
        sf::VertexArray m_vertices;
        std::string m_tileSet;
        std::vector<int> m_tiles;
        sf::Sprite m_background;
        std::map<int, std::unique_ptr<Entity>> m_entities;
        std::map<std::type_index, std::unique_ptr<CList>> m_componentLists;
        int m_playerId;
        std::vector<int> m_deletingEntities;
        std::vector<std::pair<std::type_index, int>> m_deletingComponents;
    };
}

#endif