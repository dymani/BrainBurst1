#include "BB/Handler/GuiHandler.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/Entity.h"
#include "BB/Component/GuiComponent.h"
#include "BB/Component/GraphicsComponent.h"

namespace bb {
    GuiHandler::GuiHandler(WindowHandler& windowHandler, std::vector<Entity*>& entities): m_windowHandler(windowHandler), m_entities(entities) {
    }

    int GuiHandler::update() {
        m_guiEntities.clear();
        int i = -1;
        for(auto& entity : m_entities) {
            auto gc = entity->get<GuiComponent>();
            if(!gc)
                continue;
            m_guiEntities[gc->getId()] = entity;
            if(i != -1)
                continue;
            if(gc->update(entity->get<GraphicsComponent>())) {
                i = gc->getId();
            }
        }
        return i;
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

    Entity* GuiHandler::getEntity(int id) {
        if(m_guiEntities.find(id) == m_guiEntities.end()) {
            LogHandler::log(LogHandler::ERR, "Gui entity id " + std::to_string(id) + " not found",
                typeid(*this).name());
            return nullptr;
        }
        return m_guiEntities[id];
    }

}