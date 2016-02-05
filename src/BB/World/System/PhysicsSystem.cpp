#include "BB/World/System/PhysicsSystem.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/World/LuaEntity.h"

namespace bb {
    PhysicsSystem::PhysicsSystem(GameStateGame& game) : m_game(game) {
    }

    void PhysicsSystem::createList(std::map<std::type_index, std::unique_ptr<CList>>& lists) {
        lists[std::type_index(typeid(PhysicsComponent))] = std::unique_ptr<CList>(new CList());
    }

    void PhysicsSystem::createComponent(luabridge::LuaRef& luaE, std::map < std::type_index,
        std::unique_ptr < IComponent >> &list) {
        using namespace luabridge;
        LuaRef luaComponents = luaE["components"];
        LuaRef luaPC = luaComponents["PhysicsComponent"];
        if(!luaPC.isNil()) {
            LuaRef luaIsMovable = luaPC["isMovable"];
            LuaRef luaType = luaPC["type"];
            LuaRef luaVelocities = luaPC["velocities"];
            LuaRef luaHitbox = luaPC["hitbox"];
            LuaRef luaOnHitGround = luaPC["onHitGround"];
            LuaRef luaOnCollide = luaPC["onCollide"];
            auto* pc = new PhysicsComponent();
            pc->m_isMovable = luaIsMovable.cast<bool>();
            if(pc->m_isMovable) {
                pc->m_velocities = {luaVelocities[1].cast<float>(), luaVelocities[2].cast<float>()};
                pc->m_isOnGround = false;
                pc->m_onHitGround = std::make_shared<LuaRef>(luaOnHitGround);
            }
            pc->m_type = luaType.cast<int>();
            if(pc->m_type != 0) {
                pc->m_hitbox = {luaHitbox[1].cast<float>(), luaHitbox[2].cast<float>(),
                    luaHitbox[3].cast<float>(), luaHitbox[4].cast<float>()};
                pc->m_onCollide = std::make_shared<LuaRef>(luaOnCollide);
            }
            list[std::type_index(typeid(PhysicsComponent))] = std::unique_ptr<PhysicsComponent>(pc);
        }
    }

    void PhysicsSystem::createComponent(rapidjson::Value& jsonE,
        std::map<std::type_index, std::unique_ptr<IComponent>>& list, Entity* entity) {
        auto* mcomponent = list[std::type_index(typeid(PhysicsComponent))].get();
        if(mcomponent) {
            auto* pc = new PhysicsComponent(*dynamic_cast<PhysicsComponent*>(mcomponent));
            if(jsonE.HasMember("velocityX"))
                pc->m_velocities.x = jsonE["velocityX"].GetDouble();
            if(jsonE.HasMember("velocityY"))
                pc->m_velocities.y = jsonE["velocityY"].GetDouble();
            entity->addComponent(std::type_index(typeid(PhysicsComponent)), pc);
        }
    }

    void PhysicsSystem::update() {
        auto& pcList = m_game.getWorld().getField()->getComponentList<PhysicsComponent>()->m_list;
        Entity* e;
        for(auto& pcIA : pcList) {
            auto& pcA = *dynamic_cast<PhysicsComponent*>(pcIA.second.get());
            if(pcA.m_isMovable == false) continue;
            e = m_game.getWorld().getField()->getEntity(pcIA.first);
            if(!pcA.m_isOnGround) {
                if(pcA.m_velocities.y > -30.0F)
                    pcA.m_velocities.y -= 1.0F;
            }
            if(pcA.m_velocities.x > 0) {
                pcA.m_velocities.x -= 0.5F;
                if(pcA.m_velocities.x < 0) pcA.m_velocities.x = 0;
            } else if(pcA.m_velocities.x < 0) {
                pcA.m_velocities.x += 0.5F;
                if(pcA.m_velocities.x > 0) pcA.m_velocities.x = 0;
            }
            sf::Vector2f coord = e->getCoord();
            coord.x += pcA.m_velocities.x / 50;
            coord.y += pcA.m_velocities.y / 50;
            if(pcA.m_type == 0) {
                if(coord.y <= 0) {
                    pcA.m_isOnGround = true;
                    coord.y = 0;
                    pcA.m_velocities.y = 0;
                    try {
                        if(pcA.m_isMovable)
                            if((*pcA.m_onHitGround)(new LuaEntity(m_game, pcIA.first)).cast<bool>())
                                m_game.getWorld().getField()->addDeleteEntity(pcIA.first);
                    } catch(luabridge::LuaException const& e) {
                        LogHandler::log<PhysicsSystem>(ERR, "LuaException: ");
                        std::cout << "                " << e.what() << std::endl;
                    }
                } else {
                    pcA.m_isOnGround = false;
                }
            } else {
                if(coord.y + pcA.m_hitbox.top <= 0) {
                    pcA.m_isOnGround = true;
                    coord.y = -pcA.m_hitbox.top;
                    pcA.m_velocities.y = 0;
                    try {
                        if(pcA.m_isMovable)
                            if((*pcA.m_onHitGround)(new LuaEntity(m_game, pcIA.first)).cast<bool>())
                                m_game.getWorld().getField()->addDeleteEntity(pcIA.first);
                    } catch(luabridge::LuaException const& e) {
                        LogHandler::log<PhysicsSystem>(ERR, "LuaException: ");
                        std::cout << "                " << e.what() << std::endl;
                    }
                } else {
                    pcA.m_isOnGround = false;
                }
                for(auto& pcIB : pcList) {
                    if(pcIA.first == pcIB.first) continue;
                    auto& pcB = *dynamic_cast<PhysicsComponent*>(pcIB.second.get());
                    if(pcB.m_type == 0) return;
                    sf::Vector2f coordB = m_game.getWorld().getField()->getEntity(pcIB.first)->getCoord();
                    auto& gs = m_game.getWorld().getSystem<GraphicsSystem>();

                    sf::Vector2f ltCoordA = {coord.x + pcA.m_hitbox.left, coord.y + pcA.m_hitbox.top
                        + pcA.m_hitbox.height};
                    sf::Vector2f ltCoordB = {coordB.x + pcB.m_hitbox.left, coordB.y + pcB.m_hitbox.top
                        + pcB.m_hitbox.height};
                    sf::Vector2f sizeA = {pcA.m_hitbox.width, pcA.m_hitbox.height};
                    sf::Vector2f sizeB = {pcB.m_hitbox.width, pcB.m_hitbox.height};

                    sf::FloatRect hitboxA = {gs.mapCoordsToPixel(ltCoordA), sizeA * float(gs.getTileSize())};
                    sf::FloatRect hitboxB = {gs.mapCoordsToPixel(ltCoordB), sizeB * float(gs.getTileSize())};
                    if(!hitboxA.intersects(hitboxB)) continue;
                    sf::Vector2f newVelocities = pcA.m_velocities;
                    if(pcA.m_type == 2) {
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
                                0, bottomB + hitboxA.height}).y - pcA.m_hitbox.top;
                            newVelocities.y = 0.0F;
                        } else if(bCol < tCol && bCol < lCol && bCol < rCol) { //bottom collision
                            coord.y = m_game.getWorld().getSystem<GraphicsSystem>().mapPixelToCoords({
                                0, hitboxB.top}).y - pcA.m_hitbox.top;
                            newVelocities.y = 0.0F;
                            pcA.m_isOnGround = true;
                        } else if(lCol < rCol && lCol < tCol && lCol < bCol) { //Left collision
                            coord.x = m_game.getWorld().getSystem<GraphicsSystem>().mapPixelToCoords({
                                rightB, 0}).x - pcA.m_hitbox.left;
                            newVelocities.x = 0.0F;
                        } else if(rCol < lCol && rCol < tCol && rCol < bCol) { //Right collision
                            coord.x = m_game.getWorld().getSystem<GraphicsSystem>().mapPixelToCoords({
                                hitboxB.left - hitboxA.width, 0}).x - pcA.m_hitbox.left;
                            newVelocities.x = 0.0F;
                        }
                    }
                    try {
                        if(pcA.m_type != 0)
                            if((*pcA.m_onCollide)(new LuaEntity(m_game, pcIA.first), new LuaEntity(m_game,
                                pcIB.first)).cast<bool>())
                                m_game.getWorld().getField()->addDeleteEntity(pcIA.first);
                        if(pcB.m_type != 0)
                            if((*pcB.m_onCollide)(new LuaEntity(m_game, pcIB.first), new LuaEntity(m_game,
                                pcIA.first)).cast<bool>())
                                m_game.getWorld().getField()->addDeleteEntity(pcIB.first);
                    } catch(luabridge::LuaException const& e) {
                        LogHandler::log<PhysicsSystem>(ERR, "LuaException: ");
                        std::cout << "                " << e.what() << std::endl;
                    }
                    pcA.m_velocities = newVelocities;
                }
            }
            e->setCoord(coord);
        }
    }

    bool PhysicsSystem::contain(Entity* e, sf::Vector2f pointCoord) {
        auto* pc = m_game.getWorld().getField()->getComponent<PhysicsComponent>(e->getId());
        sf::Vector2f coord = e->getCoord();
        sf::Vector2f ltCoord = {coord.x + pc->m_hitbox.left, coord.y + pc->m_hitbox.top
            + pc->m_hitbox.height};
        auto& gs = m_game.getWorld().getSystem<GraphicsSystem>();
        sf::Vector2f size = {pc->m_hitbox.width, pc->m_hitbox.height};
        sf::FloatRect hitbox = {gs.mapCoordsToPixel(ltCoord), size * float(gs.getTileSize())};
        sf::Vector2f pointPixel = gs.mapCoordsToPixel(pointCoord);
        return hitbox.contains(pointPixel);
    }

    sf::Vector2f PhysicsSystem::getVelocity(PhysicsComponent * pc) {
        return pc->m_velocities;
    }

    void PhysicsSystem::setVelocity(PhysicsComponent* pc, sf::Vector2f velocity) {
        pc->m_velocities = velocity;
    }

    void PhysicsSystem::setHitbox(PhysicsComponent* pc, sf::FloatRect hitbox) {
        pc->m_hitbox = hitbox;
    }
}