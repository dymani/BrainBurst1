#include "BB/Handler/GraphicsHandler.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/Entity.h"

namespace bb {
    GraphicsHandler::GraphicsHandler(WindowHandler& windowHandler): m_windowHandler(windowHandler) {
        m_rect.setSize({100, 100});
    }

    void GraphicsHandler::addEntity(Entity* entity) {
        m_entities.push_back(entity);
    }

    void GraphicsHandler::draw() {
        m_rect.setPosition(0, 0);
        sf::RenderWindow& window = m_windowHandler.getWindow();
        std::sort(m_entities.begin(), m_entities.end(), compareEntities);
        for(auto& entity : m_entities) {
            m_rect.move(50, 50);
            auto gc = entity->get<GraphicsComponent>();
            m_rect.setFillColor(gc->getColor());
            window.draw(m_rect);
        }
    }

    bool compareEntities(Entity* a, Entity* b) {
        return (a->getZ() < b->getZ());
    }
}