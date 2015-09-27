#ifndef GUI_HANDLER_H
#define GUI_HANDLER_H

#include <SFML/Graphics.hpp>

namespace bb {
    class WindowHandler;
    class Entity;

    class GuiHandler {
    public:
        GuiHandler(WindowHandler& windowHandler, std::vector<Entity*>& entities);
        int update();
        void handleInput(sf::Event& windowEvent);
    private:
        WindowHandler& m_windowHandler;
        std::vector<Entity*>& m_entities;
    };
}

#endif