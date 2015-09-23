#include "BB/Handler/GraphicsHandler.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/Entity.h"

namespace bb {
    GraphicsHandler::GraphicsHandler(WindowHandler& windowHandler): m_windowHandler(windowHandler) {

    }

    void GraphicsHandler::addEntity(Entity* entity) {
        m_entities.push_back(entity);
    }

    void GraphicsHandler::draw(const double dt) {
        sf::RenderWindow& window = m_windowHandler.getWindow();
        std::sort(m_entities.begin(), m_entities.end(), compareEntities);
        for(auto& entity : m_entities) {
            auto gc = entity->get<GraphicsComponent>();
            sf::Sprite& sprite = gc->getSprite();
            sprite.setPosition(sf::Vector2f(entity->getCoord()));
            window.draw(sprite);
        }
    }

    bool compareEntities(Entity* a, Entity* b) {
        return (a->getZ() < b->getZ());
    }
}