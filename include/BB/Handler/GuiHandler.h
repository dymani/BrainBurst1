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
        Entity* getEntity(int id);
    private:
        WindowHandler& m_windowHandler;
        std::vector<Entity*>& m_entities;
        std::map<int, Entity*> m_guiEntities;
    };
}

#endif