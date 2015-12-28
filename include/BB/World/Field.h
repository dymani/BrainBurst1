#ifndef FIELD_H
#define FIELD_H

#include "BB/World/Entity.h"
#include "BB/World/EntityTemplate.h"

namespace bb {
    class GameStateGame;

    class Field {
    public:
        Field(GameStateGame& game, std::string worldName, std::string id);
        void load();
        void handleInput();
        void handleInput(sf::Event& windowEvent);
        void update();
        void draw(const double dt);
        int getSize();
        Entity* getEntity(int id);
        IComponent getComponent(std::type_index type, int id);
        std::map<int, IComponent*>& getComponentList(std::type_index type);
    private:
        GameStateGame& m_game;
        std::string m_worldName, m_id;
        sf::RenderStates m_states;
        sf::VertexArray m_vertices;
        std::string m_tileSet;
        std::vector<int> m_tiles;
        sf::Sprite m_background;
        std::map<std::string, EntityTemplate*> m_entityTemplates;
        std::map<int, Entity*> m_entities;
        std::map<std::type_index, std::map<int, IComponent*>*> m_componentLists;
        std::map<int, IComponent*> m_graphicsComponents;
        std::map<int, IComponent*> m_movementComponents;
    };
}

#endif