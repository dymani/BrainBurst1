#ifndef GRAPHICS_HANDLER_H
#define GRAPHICS_HANDLER_H

#include <SFML/Graphics.hpp>

namespace bb {
    class WindowHandler;
    class Entity;

    class GraphicsHandler {
    public:
        GraphicsHandler(WindowHandler& windowHandler);
        void addEntity(Entity* entity);
        void draw(const double dt);
    private:
        WindowHandler& m_windowHandler;
        std::vector<Entity*> m_entities;
    };
    bool compareEntities(Entity* a, Entity* b);
}

#endif