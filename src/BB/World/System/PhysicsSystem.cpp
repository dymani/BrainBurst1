#include "BB/World/System/PhysicsSystem.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/World/LuaEntity.h"

namespace bb {
    PhysicsSystem::PhysicsSystem(GameStateGame& game) : m_game(game) {
    }

    void PhysicsSystem::createList(std::map<std::type_index, std::unique_ptr<CList>>& lists) {
        lists[std::type_index(typeid(MovementComponent))] = std::unique_ptr<CList>(new CList());
        lists[std::type_index(typeid(CollisionComponent))] = std::unique_ptr<CList>(new CList());
    }

    void PhysicsSystem::createComponent(luabridge::LuaRef& luaE, std::map<std::type_index,
        std::unique_ptr<IComponent>>& list) {
        using namespace luabridge;
        LuaRef luaComponents = luaE["components"];
        LuaRef luaMC = luaComponents["MovementComponent"];
        if(!luaMC.isNil()) {
            LuaRef luaVelocities = luaMC["velocities"];
            auto* mc = new MovementComponent();
            mc->m_velocities = {luaVelocities[1].cast<float>(), luaVelocities[2].cast<float>()};
            mc->m_isOnGround = false;
            list[std::type_index(typeid(MovementComponent))] = std::unique_ptr<MovementComponent>(mc);
        }
        LuaRef luaCC = luaComponents["CollisionComponent"];
        if(!luaCC.isNil()) {
            LuaRef luaHitbox = luaCC["hitbox"];
            LuaRef luaCollideFunc = luaCC["collide"];
            auto* cc = new CollisionComponent();
            cc->m_hitbox = {luaHitbox[1].cast<float>(), luaHitbox[2].cast<float>(),
                luaHitbox[3].cast<float>(), luaHitbox[4].cast<float>()};
            cc->m_collideFunc = std::make_shared<LuaRef>(luaCollideFunc);
            list[std::type_index(typeid(CollisionComponent))] = std::unique_ptr<CollisionComponent>(cc);
        }
    }

    void PhysicsSystem::createComponent(rapidjson::Value& jsonE,
        std::map<std::type_index, std::unique_ptr<IComponent>>& list, Entity* entity) {
        auto* mcomponent = list[std::type_index(typeid(MovementComponent))].get();
        if(mcomponent) {
            auto* mc = new MovementComponent(*dynamic_cast<MovementComponent*>(mcomponent));
            if(jsonE.HasMember("velocityX"))
                mc->m_velocities.x = jsonE["velocityX"].GetDouble();
            if(jsonE.HasMember("velocityY"))
                mc->m_velocities.y = jsonE["velocityY"].GetDouble();
            entity->addComponent(std::type_index(typeid(MovementComponent)), mc);
        }
        auto* ccomponent = list[std::type_index(typeid(CollisionComponent))].get();
        if(ccomponent) {
            auto* cc = new CollisionComponent(*dynamic_cast<CollisionComponent*>(ccomponent));
            entity->addComponent(std::type_index(typeid(CollisionComponent)), cc);
        }
    }

    void PhysicsSystem::update() {
        auto& mcList = m_game.getWorld().getField()->getComponentList<MovementComponent>()->m_list;
        auto& ccList = m_game.getWorld().getField()->getComponentList<CollisionComponent>()->m_list;
        Entity* e;
        for(auto& mcI : mcList) {
            auto& mc = *dynamic_cast<MovementComponent*>(mcI.second.get());
            e = m_game.getWorld().getField()->getEntity(mcI.first);
            if(!mc.m_isOnGround) {
                if(mc.m_velocities.y > -30.0F)
                    mc.m_velocities.y -= 1.0F;
            }
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
            if(ccList.find(mcI.first) == ccList.end()) {
                if(e->getCoord().y >= 0 && coord.y <= 0) {
                    mc.m_isOnGround = true;
                    coord.y = 0;
                } else {
                    mc.m_isOnGround = false;
                }
            } else {
                auto& ccA = *dynamic_cast<CollisionComponent*>(ccList[mcI.first].get());
                if(e->getCoord().y + ccA.m_hitbox.top >= 0 && coord.y + ccA.m_hitbox.top <= 0) {
                    mc.m_isOnGround = true;
                    coord.y = -ccA.m_hitbox.top;
                } else {
                    mc.m_isOnGround = false;
                }
                for(auto& ccI : ccList) {
                    if(ccI.first == mcI.first) continue;
                    auto& ccB = *dynamic_cast<CollisionComponent*>(ccI.second.get());
                    sf::Vector2f coordB = m_game.getWorld().getField()->getEntity(ccI.first)->getCoord();
                    auto& gs = m_game.getWorld().getSystem<GraphicsSystem>();

                    sf::Vector2f ltCoordA = {coord.x + ccA.m_hitbox.left, coord.y + ccA.m_hitbox.top
                        + ccA.m_hitbox.height};
                    sf::Vector2f ltCoordB = {coordB.x + ccB.m_hitbox.left, coordB.y + ccB.m_hitbox.top
                        + ccB.m_hitbox.height};
                    sf::Vector2f sizeA = {ccA.m_hitbox.width, ccA.m_hitbox.height};
                    sf::Vector2f sizeB = {ccB.m_hitbox.width, ccB.m_hitbox.height};

                    sf::FloatRect hitboxA = {gs.mapCoordsToPixel(ltCoordA), sizeA * float(gs.getTileSize())};
                    sf::FloatRect hitboxB = {gs.mapCoordsToPixel(ltCoordB), sizeB * float(gs.getTileSize())};
                    if(!hitboxA.intersects(hitboxB)) continue;
                    float bottomA = hitboxA.top + hitboxA.height;
                    float bottomB = hitboxB.top + hitboxB.height;
                    float rightA = hitboxA.left + hitboxA.width;
                    float rightB = hitboxB.left + hitboxB.width;

                    float bCol = bottomA - hitboxB.top;
                    float tCol = bottomB - hitboxA.top;
                    float lCol = rightB - hitboxA.left;
                    float rCol = rightA - hitboxB.left;

                    if(tCol < bCol && tCol < lCol && tCol < rCol) { //Top collision
                        coord.y = m_game.getWorld().getSystem<GraphicsSystem>().mapPixelToCoords({
                            0, bottomB + hitboxA.height}).y - ccA.m_hitbox.top;
                        mc.m_velocities.y = 0.0F;
                    } else if(bCol < tCol && bCol < lCol && bCol < rCol) { //bottom collision
                        coord.y = m_game.getWorld().getSystem<GraphicsSystem>().mapPixelToCoords({
                            0, hitboxB.top}).y - ccA.m_hitbox.top;
                        mc.m_velocities.y = 0.0F;
                        mc.m_isOnGround = true;
                    } else if(lCol < rCol && lCol < tCol && lCol < bCol) { //Left collision
                        coord.x = m_game.getWorld().getSystem<GraphicsSystem>().mapPixelToCoords({
                            rightB, 0}).x - ccA.m_hitbox.left;
                    } else if(rCol < lCol && rCol < tCol && rCol < bCol) { //Right collision
                        coord.x = m_game.getWorld().getSystem<GraphicsSystem>().mapPixelToCoords({
                            hitboxB.left - hitboxA.width, 0}).x - ccA.m_hitbox.left;
                    }
                    try {
                        if((*ccA.m_collideFunc)(new LuaEntity(m_game, mcI.first)).cast<bool>())
                            m_game.getWorld().getField()->addDeleteEntity(mcI.first);
                        if((*ccB.m_collideFunc)(new LuaEntity(m_game, ccI.first)).cast<bool>())
                            m_game.getWorld().getField()->addDeleteEntity(ccI.first);
                    } catch(luabridge::LuaException const& e) {
                        LogHandler::log<PhysicsSystem>(ERR, "LuaException: ");
                        std::cout << "                " << e.what() << std::endl;
                    }

                }
            }
            e->setCoord(coord);
        }
    }

    bool PhysicsSystem::contain(Entity* e, sf::Vector2f pointCoord) {
        auto* cc = m_game.getWorld().getField()->getComponent<CollisionComponent>(e->getId());
        sf::Vector2f coord = e->getCoord();
        sf::Vector2f ltCoord = {coord.x + cc->m_hitbox.left, coord.y + cc->m_hitbox.top
            + cc->m_hitbox.height};
        auto& gs = m_game.getWorld().getSystem<GraphicsSystem>();
        sf::Vector2f size = {cc->m_hitbox.width, cc->m_hitbox.height};
        sf::FloatRect hitbox = {gs.mapCoordsToPixel(ltCoord), size * float(gs.getTileSize())};
        sf::Vector2f pointPixel = gs.mapCoordsToPixel(pointCoord);
        return hitbox.contains(pointPixel);
    }
}