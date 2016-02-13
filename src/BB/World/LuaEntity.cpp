#include "BB/World/LuaEntity.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/World/World.h"
#include "BB/World/Field.h"
#include "BB/World/System/GraphicsSystem.h"
#include "BB/World/System/HealthSystem.h"

namespace bb {
    void LuaEntity::registerState(luabridge::lua_State* L) {
        luabridge::getGlobalNamespace(L)
            .beginClass<LuaEntity>("LuaEntity")
            .addProperty("x", &LuaEntity::getCoordX, &LuaEntity::setCoordX)
            .addProperty("y", &LuaEntity::getCoordY, &LuaEntity::setCoordY)
            .addFunction("gsSetAnimation", &LuaEntity::gsSetAnimation)
            .addFunction("gsSetAnimationFrame", &LuaEntity::gsSetAnimationFrame)
            .addProperty("hsHealth", &LuaEntity::hsGetHealth)
            .addFunction("hsSetDamage", &LuaEntity::hsSetDamage)
            .addFunction("print", &LuaEntity::print)
            .addProperty("psVelocityX", &LuaEntity::psGetVelocityX, &LuaEntity::psSetVelocityX)
            .addProperty("psVelocityY", &LuaEntity::psGetVelocityY, &LuaEntity::psSetVelocityY)
            .addFunction("psSetHitbox", &LuaEntity::psSetHitbox)
            .addProperty("psOnGround", &LuaEntity::psGetOnGround, &LuaEntity::psSetOnGround)
            .addData("id", &LuaEntity::m_entity)
            .addFunction("create", &LuaEntity::createEntity)
            .endClass();
    }

    LuaEntity::LuaEntity(GameStateGame& game, int entity) : m_game(game), m_entity(entity) {
    }

    void LuaEntity::hsSetDamage(int damage) {
        auto* hc = m_game.getWorld().getField()->getComponent<HealthComponent>(m_entity);
        if(!hc) return;
        m_game.getWorld().getSystem<HealthSystem>().addDamage(hc, damage);
    }

    int LuaEntity::hsGetHealth() const {
        auto* hc = m_game.getWorld().getField()->getComponent<HealthComponent>(m_entity);
        if(!hc) return -1;
        return m_game.getWorld().getSystem<HealthSystem>().getHealth(hc);
    }

    void LuaEntity::gsSetAnimation(std::string name) {
        auto* gc = m_game.getWorld().getField()->getComponent<GraphicsComponent>(m_entity);
        if(!gc) return;
        m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, name);
    }

    void LuaEntity::gsSetAnimationFrame(int frame) {
        auto* gc = m_game.getWorld().getField()->getComponent<GraphicsComponent>(m_entity);
        if(!gc) return;
        m_game.getWorld().getSystem<GraphicsSystem>().setAnimation(gc, "", frame);
    }

    void LuaEntity::psSetVelocityX(float x) {
        auto* pc = m_game.getWorld().getField()->getComponent<PhysicsComponent>(m_entity);
        if(!pc) return;
        float y = m_game.getWorld().getSystem<PhysicsSystem>().getVelocity(pc).y;
        m_game.getWorld().getSystem<PhysicsSystem>().setVelocity(pc, {x, y});
    }

    void LuaEntity::psSetVelocityY(float y) {
        auto* pc = m_game.getWorld().getField()->getComponent<PhysicsComponent>(m_entity);
        if(!pc) return;
        float x = m_game.getWorld().getSystem<PhysicsSystem>().getVelocity(pc).x;
        m_game.getWorld().getSystem<PhysicsSystem>().setVelocity(pc, {x, y});
    }

    void LuaEntity::psSetOnGround(bool onGround) {
        auto* pc = m_game.getWorld().getField()->getComponent<PhysicsComponent>(m_entity);
        if(!pc) return;
        m_game.getWorld().getSystem<PhysicsSystem>().setOnGround(pc, onGround);
    }

    bool LuaEntity::psGetOnGround() const {
        auto* pc = m_game.getWorld().getField()->getComponent<PhysicsComponent>(m_entity);
        if(!pc) return false;
        return m_game.getWorld().getSystem<PhysicsSystem>().getOnGround(pc);
    }

    float LuaEntity::psGetVelocityX() const {
        auto* pc = m_game.getWorld().getField()->getComponent<PhysicsComponent>(m_entity);
        if(!pc) return 0;
        return m_game.getWorld().getSystem<PhysicsSystem>().getVelocity(pc).x;
    }

    float LuaEntity::psGetVelocityY() const {
        auto* pc = m_game.getWorld().getField()->getComponent<PhysicsComponent>(m_entity);
        if(!pc) return 0;
        return m_game.getWorld().getSystem<PhysicsSystem>().getVelocity(pc).y;
    }

    void LuaEntity::psSetHitbox(float x, float y, float width, float height) {
        auto* pc = m_game.getWorld().getField()->getComponent<PhysicsComponent>(m_entity);
        if(!pc) return;
        m_game.getWorld().getSystem<PhysicsSystem>().setHitbox(pc, {x, y, width, height});
    }

    void LuaEntity::setCoordX(float x) {
        auto* e = m_game.getWorld().getField()->getEntity(m_entity);
        if(!e) return;
        e->setCoord({x, e->getCoord().y});
    }

    void LuaEntity::setCoordY(float y) {
        auto* e = m_game.getWorld().getField()->getEntity(m_entity);
        if(!e) return;
        e->setCoord({e->getCoord().x, y});
    }

    float LuaEntity::getCoordX() const {
        auto* e = m_game.getWorld().getField()->getEntity(m_entity);
        if(!e) return 0;
        return e->getCoord().x;
    }

    float LuaEntity::getCoordY() const {
        auto* e = m_game.getWorld().getField()->getEntity(m_entity);
        if(!e) return 0;
        return e->getCoord().y;
    }

    int LuaEntity::getId() const {
        return m_entity;
    }

    void LuaEntity::print(std::string text) {
        LogHandler::log<LuaEntity>(INF, text);
    }

    void LuaEntity::createEntity(std::string name, float x, float y) {
        m_game.getWorld().getField()->createEntity(name, {x, y});
    }
}