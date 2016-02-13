#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include "BB/World/System/ISystem.h"
#include "BB/World/Component/PhysicsComponent.h"

namespace bb {
    class GameStateGame;

    class PhysicsSystem : public ISystem {
    public:
        PhysicsSystem(GameStateGame& game);
        void createList(std::map<std::type_index, std::unique_ptr<CList>>& lists);
        void createComponent(luabridge::LuaRef& luaE,
            std::map<std::type_index, std::unique_ptr<IComponent>>& list);
        void createComponent(rapidjson::Value& jsonE,
            std::map<std::type_index, std::unique_ptr<IComponent>>& list, Entity* entity);
        void createComponent(std::map<std::type_index, std::unique_ptr<IComponent>>& list, Entity* entity);
        void update();
        bool contain(Entity* e, sf::Vector2f coord);
        sf::Vector2f getVelocity(PhysicsComponent* pc);
        bool getOnGround(PhysicsComponent* pc);
        void setOnGround(PhysicsComponent* pc, bool onGround);
        void setVelocity(PhysicsComponent* pc, sf::Vector2f velocity);
        void setHitbox(PhysicsComponent* pc, sf::FloatRect hitbox);
    private:
        GameStateGame& m_game;
    };
}

#endif