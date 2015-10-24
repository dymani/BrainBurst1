#include "BB/Handler/GraphicsHandler.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Component/GuiComponent.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/World/Entity.h"

namespace bb {
    GraphicsHandler::GraphicsHandler(WindowHandler& windowHandler): m_windowHandler(windowHandler) {
    }

    void GraphicsHandler::draw(Entity* entity) {
        m_entities.push_back(entity);
    }

    void GraphicsHandler::display(const double dt) {
        sf::RenderWindow& window = m_windowHandler.getWindow();
        std::vector<Entity*> entities;
        for(auto& entity : m_entities) {
            if(!entity)
                continue;
            if(entity->getComponent<GraphicsComponent>())
                entities.push_back(entity);
        }
        m_entities.clear();
        std::sort(entities.begin(), entities.end(), compareEntities);
        for(auto& entity : entities) {
            entity->getComponent<GraphicsComponent>()->draw(window, {0, 0});
        }
    }

    bool compareEntities(Entity* a, Entity* b) {
        return (a->getComponent<GraphicsComponent>()->getZ() < b->getComponent<GraphicsComponent>()->getZ());
    }
}