#include "BB/World/System/PhysicsSystem.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    PhysicsSystem::PhysicsSystem(GameStateGame& game) : m_game(game) {

    }

    void PhysicsSystem::createComponent(luabridge::LuaRef& luaE, std::map<std::type_index,
        IComponent*>& list) {
        using namespace luabridge;
        LuaRef luaComponents = luaE["components"];
        LuaRef luaMC = luaComponents["MovementComponent"];
        if(luaMC.isNil()) return;
        LuaRef luaVelocities = luaMC["velocities"];
        auto* mc = new MovementComponent();
        mc->m_velocities = {luaVelocities[1].cast<float>(), luaVelocities[2].cast<float>()};
        mc->m_isOnGround = false;
        list[std::type_index(typeid(MovementComponent))] = mc;
    }

    void PhysicsSystem::createComponent(rapidjson::Value& jsonE, std::map<std::type_index, IComponent*>& list,
        Entity* entity) {
        auto* component = list[std::type_index(typeid(MovementComponent))];
        if(!component) return;
        auto* mc = new MovementComponent(*dynamic_cast<MovementComponent*>(component));
        if(jsonE.HasMember("velocityX"))
            mc->m_velocities.x = jsonE["velocityX"].GetDouble();
        if(jsonE.HasMember("velocityY"))
            mc->m_velocities.y = jsonE["velocityY"].GetDouble();
        entity->addComponent(m_game, std::type_index(typeid(MovementComponent)), mc);
    }

    void PhysicsSystem::update() {
        auto& cList = *m_game.getWorld().getField()->getComponentList<MovementComponent>();
        Entity* e;
        for(auto& c : cList) {
            auto& mc = *dynamic_cast<MovementComponent*>(c.second);
            e = m_game.getWorld().getField()->getEntity(c.first);
            if(!mc.m_isOnGround)
                mc.m_velocities.y -= 1.0F;
            if(mc.m_velocities.x > 0) {
                mc.m_velocities.x -= 0.5F;
                if(mc.m_velocities.x < 0) mc.m_velocities.x = 0;
            } else if(mc.m_velocities.x < 0) {
                mc.m_velocities.x += 0.5F;
                if(mc.m_velocities.x > 0) mc.m_velocities.x = 0;
            }
            sf::Vector2f coord = e->getCoord();
            coord.x += mc.m_velocities.x / 50;
            coord.y += mc.m_velocities.y / 50;
            if(e->getCoord().y >= 0 && coord.y <= 0) {
                mc.m_isOnGround = true;
                coord.y = 0;
            }
            e->setCoord(coord);
        }
    }
}