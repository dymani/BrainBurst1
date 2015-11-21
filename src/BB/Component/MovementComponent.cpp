#include "BB/Component/MovementComponent.h"
#include "BB/World/Entity.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    MovementComponent* MovementComponent::create(GameStateGame& game, luabridge::lua_State* L,
        luabridge::LuaRef& luaMC) {
        MovementComponent* mc = new MovementComponent(game, -1);
        using namespace luabridge;
        LuaRef luaVelocity = luaMC["velocity"];
        mc->m_velocity = {luaVelocity[1].cast<float>(), luaVelocity[2].cast<float>()};
        mc->m_isOnGround = false;
        return mc;
    }

    MovementComponent::MovementComponent(GameStateGame& game, int entity) : IComponent(game, entity) {
    }

    IComponent* MovementComponent::copy(rapidjson::Value& value) {
        MovementComponent* mc;
        if(value.HasMember("id")) {
            mc = dynamic_cast<MovementComponent*>(copy(value["id"].GetInt()));
        } else {
            mc = dynamic_cast<MovementComponent*>(copy(-1));
        }
        if(value.HasMember("xVelocity") && value.HasMember("yVelocity")) {
            mc->m_velocity = {float(value["xVelocity"].GetDouble()), float(value["yVelocity"].GetDouble())};
        }
        if(value.HasMember("isOnGround")) {
            mc->m_isOnGround = value["isOnGround"].GetBool();
        }
        return mc;
    }

    IComponent* MovementComponent::copy(int entity) {
        MovementComponent* mc = new MovementComponent(m_game, entity);
        mc->m_velocity = m_velocity;
        mc->m_isOnGround = m_isOnGround;
        return mc;
    }

    void MovementComponent::move() {
        m_newCoord = m_game.getWorld()->getEntity(m_entity)->getCoord();
        m_newCoord.x += m_velocity.x * 0.015625F;
        m_newCoord.y += m_velocity.y * 0.015625F;
        m_isOnGround = false;
        if(m_newCoord.y <= 0) {
            m_isOnGround = true;
            m_newCoord.y = 0;
            m_velocity.y = 0;
        }
    }

    void MovementComponent::update() {
        m_game.getWorld()->getEntity(m_entity)->setCoord(m_newCoord);
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

    void MovementComponent::addVelocity(float x, float y) {
        m_velocity.x += x;
        m_velocity.y += y;
    }

    void MovementComponent::setNewCoord(sf::Vector2f newCoord) {
        m_newCoord = newCoord;
    }

    void MovementComponent::setNewCoordX(float x) {
        m_newCoord.x = x;
    }

    void MovementComponent::setNewCoordY(float y) {
        m_newCoord.y = y;
    }

    sf::Vector2f MovementComponent::getVelocity() {
        return m_velocity;
    }

    sf::Vector2f MovementComponent::getNewCoord() {
        return m_newCoord;
    }

    void MovementComponent::isOnGround(bool isOnGround) {
        m_isOnGround = isOnGround;
    }

    bool MovementComponent::isOnGround() {
        return m_isOnGround;
    }
}