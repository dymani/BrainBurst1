#include "BB/Component/MovementComponent.h"
#include "BB/World/Entity.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    MovementComponent* MovementComponent::create(GameStateGame& game, int entity,
        luabridge::lua_State* L, luabridge::LuaRef& luaMC) {
        MovementComponent* mc = new MovementComponent(game, entity);
        using namespace luabridge;
        LuaRef luaVelocity = luaMC["velocity"];
        mc->m_velocity = {luaVelocity[1].cast<float>(), luaVelocity[2].cast<float>()};
        return mc;
    }

    MovementComponent::MovementComponent(GameStateGame& game, int entity) : IComponent(game, entity) {
    }

    IComponent* MovementComponent::copy(int entity) {
        MovementComponent* mc = new MovementComponent(m_game, entity);
        mc->m_velocity = m_velocity;
        return mc;
    }

    void MovementComponent::update(bool move) {
        if(!m_init) {
            m_newCoord = m_game.getWorld()->getEntity(m_entity)->getCoord();
            m_init = true;
        }
        if(move) {
            m_game.getWorld()->getEntity(m_entity)->setCoord(m_newCoord);
            m_newCoord.x += m_velocity.x * 0.015625F;
            m_newCoord.y += m_velocity.y * 0.015625F;
        }
    }

    void MovementComponent::setVelocity(sf::Vector2f velocity) {
        m_velocity = velocity;
    }

    void MovementComponent::setVelocityX(float x) {
        m_velocity.x = x;
    }

    void MovementComponent::setVelocityY(float y) {
        m_velocity.y = y;
    }

    void MovementComponent::addVelocity(sf::Vector2f velocity) {
        m_velocity += velocity;
    }

    void MovementComponent::addVelocity(float x, float y) {
        m_velocity.x += x;
        m_velocity.y += y;
    }

    void MovementComponent::setNewCoord(sf::Vector2f newCoord) {
        m_newCoord = newCoord;
    }

    sf::Vector2f MovementComponent::getNewCoord() {
        return m_newCoord;
    }

    sf::Vector2f MovementComponent::getVelocity() {
        return m_velocity;
    }
}