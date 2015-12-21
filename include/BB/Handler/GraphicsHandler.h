#ifndef GRAPHICS_HANDLER_H
#define GRAPHICS_HANDLER_H

#include <SFML/Graphics.hpp>

namespace bb {
    class GameStateGame;
    class Entity;

    class GraphicsHandler {
    public:
        GraphicsHandler(GameStateGame& game);
        void draw(int entity);
        void draw(Entity* entity);
        void display(const double dt);
        void setViewSize(float width, float height);
        void setViewCoord(float x, float y);
        void setViewCoordX(float x);
        void setViewCoordY(float Y);
        sf::Vector2f getViewCoord();
        sf::Vector2f getViewSize();
    private:
        GameStateGame& m_game;
        std::vector<Entity*> m_entities;
        sf::Vector2f m_viewCoord;
        sf::View m_view;
    };
    bool compareEntities(Entity* a, Entity* b);
}

#endif