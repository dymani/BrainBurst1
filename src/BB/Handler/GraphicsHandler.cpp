#include "BB/Handler/GraphicsHandler.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/Entity.h"

namespace bb {
    GraphicsHandler::GraphicsHandler(WindowHandler& windowHandler, std::vector<Entity*>& entities)
        : m_windowHandler(windowHandler), m_entities(entities) {
    }

    void GraphicsHandler::draw(const double dt) {
        sf::RenderWindow& window = m_windowHandler.getWindow();
        std::sort(m_entities.begin(), m_entities.end(), compareEntities);
        for(auto& entity : m_entities) {
            if(!entity) {
                continue;
            }
            auto gc = entity->get<GraphicsComponent>();
            if(!gc) {
                continue;
            }
            sf::Sprite& sprite = gc->getSprite();
            sprite.setPosition(sf::Vector2f(entity->getCoord()));
            window.draw(sprite);
        }
    }

    bool compareEntities(Entity* a, Entity* b) {
        auto gcA = a->get<GraphicsComponent>();
        auto gcB = b->get<GraphicsComponent>();
        return (gcA->getZ() < gcB->getZ());
    }
}