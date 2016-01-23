#include "BB/World/System/HealthSystem.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/World/Component/GraphicsComponent.h"
#include "BB/World/LuaEntity.h"

namespace bb {
    HealthSystem::HealthSystem(GameStateGame& game) : m_game(game) {
    }

    void HealthSystem::createList(std::map<std::type_index, std::unique_ptr<CList>>& lists) {
        lists[std::type_index(typeid(HealthComponent))] = std::unique_ptr<CList>(new CList());
    }

    void HealthSystem::createComponent(luabridge::LuaRef& luaE, std::map<std::type_index, std::unique_ptr<IComponent>>& list) {
        using namespace luabridge;
        LuaRef luaSize = luaE["size"];
        LuaRef luaComponents = luaE["components"];
        LuaRef luaHC = luaComponents["HealthComponent"];
        if(luaHC.isNil()) return;
        LuaRef luaMaxHealth = luaHC["maxHealth"];
        LuaRef luaHealth = luaHC["health"];
        LuaRef luaOnHit = luaHC["onHit"];
        LuaRef luaOnHealthChange = luaHC["onHealthChange"];
        LuaRef luaOnDeath = luaHC["onDeath"];
        auto* hc = new HealthComponent();
        hc->m_maxHealth = luaMaxHealth.cast<int>();
        hc->m_health = luaHealth.cast<int>();
        hc->m_damage = 0;
        hc->m_onHit = std::make_shared<LuaRef>(luaOnHit);
        hc->m_onHealthChange = std::make_shared<LuaRef>(luaOnHealthChange);
        hc->m_onDeath = std::make_shared<LuaRef>(luaOnDeath);
        list[std::type_index(typeid(HealthComponent))] = std::unique_ptr<HealthComponent>(hc);
    }

    void HealthSystem::createComponent(rapidjson::Value& jsonE,
        std::map<std::type_index, std::unique_ptr<IComponent>>& list, Entity* entity) {
        auto* component = list[std::type_index(typeid(HealthComponent))].get();
        if(!component) return;
        auto* hc = new HealthComponent(*dynamic_cast<HealthComponent*>(component));
        if(jsonE.HasMember("health"))
            hc->m_health = float(jsonE["health"].GetDouble());
        if(jsonE.HasMember("damage"))
            hc->m_damage = jsonE["damage"].GetInt();
        entity->addComponent(std::type_index(typeid(HealthComponent)), hc);
    }

    void HealthSystem::update() {
        auto& gs = m_game.getWorld().getSystem<GraphicsSystem>();
        auto& hcList = m_game.getWorld().getField()->getComponentList<HealthComponent>()->m_list;
        for(auto& c : hcList) {
            auto& hc = *dynamic_cast<HealthComponent*>(c.second.get());
            if(hc.m_damage != 0) {
                hc.m_health -= hc.m_damage;
                hc.m_damage = 0;
                try {
                    if((*hc.m_onHealthChange)(new LuaEntity(m_game, c.first)).cast<bool>()) {
                        m_game.getWorld().getField()->addDeleteEntity(c.first);
                    }
                } catch(luabridge::LuaException const& e) {
                    LogHandler::log<HealthSystem>(ERR, "LuaException: ");
                    std::cout << "                " << e.what() << std::endl;
                }
            }
            if(hc.m_health <= 0) {
                try {
                    if((*hc.m_onDeath)(new LuaEntity(m_game, c.first)).cast<bool>()) {
                        m_game.getWorld().getField()->addDeleteEntity(c.first);
                    }
                } catch(luabridge::LuaException const& e) {
                    LogHandler::log<HealthSystem>(ERR, "LuaException: ");
                    std::cout << "                " << e.what() << std::endl;
                }
            }
        }
    }

    void HealthSystem::hit(int entity) {
        auto& hc = *m_game.getWorld().getField()->getComponent<HealthComponent>(entity);
        try {
            if((*hc.m_onHit)(new LuaEntity(m_game, entity)).cast<bool>()) {
                m_game.getWorld().getField()->addDeleteEntity(entity);
            }
        } catch(luabridge::LuaException const& e) {
            LogHandler::log<HealthSystem>(ERR, "LuaException: ");
            std::cout << "                " << e.what() << std::endl;
        }
    }

    void HealthSystem::addDamage(HealthComponent* hc, int damage) {
        hc->m_damage += damage;
    }

    int HealthSystem::getHealth(HealthComponent* hc) {
        return hc->m_health;
    }
}