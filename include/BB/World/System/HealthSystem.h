#ifndef HEALTH_SYSTEM_H
#define HEALTH_SYSTEM_H

#include "BB/World/System/ISystem.h"
#include "BB/World/Component/HealthComponent.h"

namespace bb {
    class GameStateGame;

    class HealthSystem : public ISystem {
    public:
        HealthSystem(GameStateGame& game);
        void createList(std::map<std::type_index, std::unique_ptr<CList>>& lists);
        void createComponent(luabridge::LuaRef& luaE,
            std::map<std::type_index, std::unique_ptr<IComponent>>& list);
        void createComponent(rapidjson::Value& jsonE,
            std::map<std::type_index, std::unique_ptr<IComponent>>& list, Entity* entity);
        void update();
        void hit(int entity);
        void addDamage(HealthComponent* hc, int damage);
        int getHealth(HealthComponent* hc);
    private:
        GameStateGame& m_game;
    };
}

#endif