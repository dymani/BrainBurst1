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
            sf::Drawable& drawable = gc->getDrawable();
            dynamic_cast<sf::Transformable&>(drawable).setPosition(sf::Vector2f(entity->getCoord()));
            window.draw(drawable);
        }
    }

    bool compareEntities(Entity* a, Entity* b) {
        auto gcA = a->get<GraphicsComponent>();
        auto gcB = b->get<GraphicsComponent>();
        if(gcA && !gcB) {
            return true;
        } else if(!gcA && gcB) {
            return false;
        } else if(!gcA && !gcB) {
            auto gcA = a->get<GuiComponent>();
            auto gcB = b->get<GuiComponent>();
            return (gcA->getType() < gcB->getType());
        } else {
            return (gcA->getZ() < gcB->getZ());
        }
    }
}