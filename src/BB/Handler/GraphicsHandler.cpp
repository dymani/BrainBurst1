#include "BB/Handler/GraphicsHandler.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Component/GuiComponent.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/Entity.h"

namespace bb {
    GraphicsHandler::GraphicsHandler(WindowHandler& windowHandler, std::vector<Entity*>& entities)
        : m_windowHandler(windowHandler), m_entities(entities) {
    }

    void GraphicsHandler::draw(const double dt) {
        sf::RenderWindow& window = m_windowHandler.getWindow();
        std::vector<Entity*> entities;
        for(auto& entity : m_entities) {
            if(!entity)
                continue;
            if(entity->get<GraphicsComponent>())
                entities.push_back(entity);
        }
        std::sort(entities.begin(), entities.end(), compareEntities);
        for(auto& entity : entities) {
            auto gc = entity->get<GraphicsComponent>();
            if(!gc) {
                continue;
            }
            sf::Transform transform;
            transform.translate(entity->getCoord());
            for(auto& drawable : gc->getDrawables()) {
                window.draw(*drawable.second->m_drawable, transform);
            }
        }
    }

    bool compareEntities(Entity* a, Entity* b) {
        auto gcA = a->get<GraphicsComponent>();
        auto gcB = b->get<GraphicsComponent>();
        if(gcA && gcB) {
            return (gcA->getZ() < gcB->getZ());
        }
        return true;
    }
}