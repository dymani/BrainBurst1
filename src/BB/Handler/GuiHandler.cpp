#include "BB/Handler/GuiHandler.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/Entity.h"
#include "BB/Component/GuiComponent.h"
#include "BB/Component/GraphicsComponent.h"

namespace bb {
    GuiHandler::GuiHandler(WindowHandler& windowHandler, std::vector<Entity*>& entities): m_windowHandler(windowHandler), m_entities(entities) {
    }

    int GuiHandler::update() {
        for(auto& entity : m_entities) {
            auto gc = entity->get<GuiComponent>();
            if(!gc)
                continue;
            if(gc->update(entity->get<GraphicsComponent>())) {
                return gc->getId();
            }
        }
        return -1;
    }

    void GuiHandler::handleInput(sf::Event& windowEvent) {
        for(auto& entity : m_entities) {
            auto gc = entity->get<GuiComponent>();
            if(!gc)
                continue;
            if(gc->handleInput(windowEvent, m_windowHandler.getWindow(), entity->getCoord(),
                entity->get<GraphicsComponent>()))
                return;
        }
    }
}