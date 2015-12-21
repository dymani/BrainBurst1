#include "BB/World/Entity.h"
#include "BB/Handler/GraphicsHandler.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Handler/WindowHandler.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/Component/MovementComponent.h"

namespace bb {
    GraphicsHandler::GraphicsHandler(GameStateGame& game) : m_game(game) {
        m_view = m_game.getWindowHandler()->getWindow().getDefaultView();
    }

    void GraphicsHandler::draw(int entity) {
        m_entities.push_back(m_game.getWorld()->getEntity(entity));
    }

    void GraphicsHandler::draw(Entity* entity) {
        m_entities.push_back(entity);
    }

    void GraphicsHandler::display(const double dt) {
        sf::RenderWindow& window = m_game.getWindowHandler()->getWindow();
        std::vector<Entity*> entities;
        for(auto& entity : m_entities) {
            if(entity)
                if(entity->getComponent<GraphicsComponent>())
                    entities.push_back(entity);
        }
        window.setView(m_view);
        m_entities.clear();
        std::sort(entities.begin(), entities.end(), compareEntities);
        for(auto& entity : entities) {
            if(entity->getComponent<MovementComponent>()) {
                //entity->getComponent<GraphicsComponent>()->draw(window, {
                //    float(entity->getComponent<MovementComponent>()->getVelocity().x * dt),
                //    float(entity->getComponent<MovementComponent>()->getVelocity().y * dt)});
                entity->getComponent<GraphicsComponent>()->draw(window, {0, 0});
            }
            entity->getComponent<GraphicsComponent>()->draw(window);
        }
    }

    void GraphicsHandler::setViewSize(float width, float height) {
        m_view.setSize({width, height});
    }

    void GraphicsHandler::setViewCoord(float x, float y) {
        m_viewCoord = {x, y};
        if(x * 64 - m_view.getSize().x / 2 < 0)
            m_viewCoord.x = m_view.getSize().x / 2 / 64;
        if(x * 64 + m_view.getSize().x / 2 > m_game.getWorld()->getField()->getSize() * 64)
            m_viewCoord.x = m_game.getWorld()->getField()->getSize() - m_view.getSize().x / 2 / 64;
        if(y < 0)
            m_viewCoord.y = 0;
        m_view.setCenter(m_viewCoord.x * 64, m_view.getSize().y / 2 - m_viewCoord.y * 64);
    }

    void GraphicsHandler::setViewCoordX(float x) {
        setViewCoord(x, m_viewCoord.y);
    }

    void GraphicsHandler::setViewCoordY(float y) {
        setViewCoord(m_viewCoord.x, y);
    }

    sf::Vector2f GraphicsHandler::getViewCoord() {
        return m_viewCoord;
    }

    sf::Vector2f GraphicsHandler::getViewSize() {
        return m_view.getSize();
    }

    bool compareEntities(Entity* a, Entity* b) {
        return (a->getComponent<GraphicsComponent>()->getZ() < b->getComponent<GraphicsComponent>()->getZ());
    }
}