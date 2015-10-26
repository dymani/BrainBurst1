#ifndef GRAPHICS_HANDLER_H
#define GRAPHICS_HANDLER_H

#include <SFML/Graphics.hpp>

namespace bb {
    class GameStateGame;

    class GraphicsHandler {
    public:
        GraphicsHandler(GameStateGame& game);
        void draw(int entity);
        void display(const double dt);
    private:
        GameStateGame& m_game;
        std::vector<int> m_entities;
    };
    bool compareEntities(Entity* a, Entity* b);
}

#endif