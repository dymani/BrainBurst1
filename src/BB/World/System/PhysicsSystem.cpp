#include "BB/World/System/PhysicsSystem.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    PhysicsSystem::PhysicsSystem(GameStateGame& game) : m_game(game) {

    }

    void PhysicsSystem::createList(std::map<std::type_index, std::map<int, IComponent*>*>& lists) {
        lists[std::type_index(typeid(MovementComponent))] = new std::map<int, IComponent*>;
        lists[std::type_index(typeid(CollisionComponent))] = new std::map<int, IComponent*>;
    }

    void PhysicsSystem::createComponent(luabridge::LuaRef& luaE, std::map<std::type_index,
        IComponent*>& list) {
        using namespace luabridge;
        LuaRef luaComponents = luaE["components"];
        LuaRef luaMC = luaComponents["MovementComponent"];
        if(!luaMC.isNil()) {
            LuaRef luaVelocities = luaMC["velocities"];
            auto* mc = new MovementComponent();
            mc->m_velocities = {luaVelocities[1].cast<float>(), luaVelocities[2].cast<float>()};
            mc->m_isOnGround = false;
            list[std::type_index(typeid(MovementComponent))] = mc;
        }
        LuaRef luaCC = luaComponents["CollisionComponent"];
        if(!luaCC.isNil()) {
            LuaRef luaHitbox = luaCC["hitbox"];
            LuaRef luaCollideFunc = luaCC["collide"];
            auto* cc = new CollisionComponent();
            cc->m_hitbox = {luaHitbox[1].cast<float>(), luaHitbox[2].cast<float>(),
                luaHitbox[3].cast<float>(), luaHitbox[4].cast<float>()};
            cc->m_collideFunc = std::make_shared<LuaRef>(luaCollideFunc);
            list[std::type_index(typeid(CollisionComponent))] = cc;
        }
    }

    void PhysicsSystem::createComponent(rapidjson::Value& jsonE, std::map<std::type_index, IComponent*>& list,
        Entity* entity) {
        auto* component = list[std::type_index(typeid(MovementComponent))];
        if(component) {
            auto* mc = new MovementComponent(*dynamic_cast<MovementComponent*>(component));
            if(jsonE.HasMember("velocityX"))
                mc->m_velocities.x = jsonE["velocityX"].GetDouble();
            if(jsonE.HasMember("velocityY"))
                mc->m_velocities.y = jsonE["velocityY"].GetDouble();
            entity->addComponent(m_game, std::type_index(typeid(MovementComponent)), mc);
        }
        component = list[std::type_index(typeid(CollisionComponent))];
        if(component) {
            auto* cc = new CollisionComponent(*dynamic_cast<CollisionComponent*>(component));
            entity->addComponent(m_game, std::type_index(typeid(CollisionComponent)), cc);
        }
    }

    void PhysicsSystem::update() {
        auto& mcList = *m_game.getWorld().getField()->getComponentList<MovementComponent>();
        auto& ccList = *m_game.getWorld().getField()->getComponentList<CollisionComponent>();
        Entity* e;
        for(auto& mcI : mcList) {
            auto& mc = *dynamic_cast<MovementComponent*>(mcI.second);
            e = m_game.getWorld().getField()->getEntity(mcI.first);
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
            if(ccList.find(mcI.first) == ccList.end()) {
                if(e->getCoord().y >= 0 && coord.y <= 0) {
                    mc.m_isOnGround = true;
                    coord.y = 0;
                } else {
                    mc.m_isOnGround = false;
                }
            }else{
                auto& ccA = *dynamic_cast<CollisionComponent*>(ccList[mcI.first]);
                if(e->getCoord().y + ccA.m_hitbox.top >= 0 && coord.y + ccA.m_hitbox.top <= 0) {
                    mc.m_isOnGround = true;
                    coord.y = -ccA.m_hitbox.top;
                } else {
                    mc.m_isOnGround = false;
                }
                for(auto& ccI : ccList) {
                    if(ccI.first == mcI.first) continue;
                    auto& ccB = *dynamic_cast<CollisionComponent*>(ccI.second);
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
                            0, hitboxB.top + hitboxB.height + hitboxA.height}).y + ccA.m_hitbox.top;
                        mc.m_velocities.y = 0.0F;
                    } else if(bCol < tCol && bCol < lCol && bCol < rCol) { //bottom collision
                        coord.y = m_game.getWorld().getSystem<GraphicsSystem>().mapPixelToCoords({
                            0, hitboxB.top}).y - ccA.m_hitbox.top;
                        mc.m_velocities.y = 0.0F;
                        mc.m_isOnGround = true;
                    } else if(lCol < rCol && lCol < tCol && lCol < bCol) { //Left collision
                        coord.x = m_game.getWorld().getSystem<GraphicsSystem>().mapPixelToCoords({
                            hitboxB.left + hitboxB.width, 0}).x - ccA.m_hitbox.left;
                    } else if(rCol < lCol && rCol < tCol && rCol < bCol) { //Right collision
                        coord.x = m_game.getWorld().getSystem<GraphicsSystem>().mapPixelToCoords({
                            hitboxB.left - hitboxA.width, 0}).x - ccA.m_hitbox.left;
                    }

                    try {
                        if((*ccA.m_collideFunc)().cast<bool>())
                            std::cout << "Delete A\n" ;
                        if((*ccB.m_collideFunc)().cast<bool>())
                            std::cout << "Delete B\n" ;
                    } catch(luabridge::LuaException const& e) {
                        LogHandler::log<PhysicsSystem>(ERR, "LuaException: ");
                        std::cout << "                " << e.what() << std::endl;
                    }

                }
            }
            e->setCoord(coord);
        }
    }
}