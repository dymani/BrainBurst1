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
        LuaRef luaFrames = luaHC["frames"];
        LuaRef luaDeathFunc = luaHC["death"];
        auto* hc = new HealthComponent();
        hc->m_maxHealth = luaMaxHealth.cast<int>();
        hc->m_health = luaHealth.cast<int>();
        hc->m_damage = 0;
        if(luaFrames.length() == 1) {
            for(int i = 0; i <= hc->m_maxHealth; i++)
                hc->m_frames[i] = luaFrames[1].cast<std::string>();
        } else if(luaFrames.length() == 2) {
            hc->m_frames[hc->m_maxHealth] = luaFrames[1].cast<std::string>();
            for(int i = 0; i < hc->m_maxHealth; i++)
                hc->m_frames[i] = luaFrames[2].cast<std::string>();
        } else if(luaFrames.length() == 3) {
            hc->m_frames[hc->m_maxHealth] = luaFrames[1].cast<std::string>();
            hc->m_frames[0] = luaFrames[3].cast<std::string>();
            hc->m_frames[1] = luaFrames[3].cast<std::string>();
            for(int i = 2; i < hc->m_maxHealth; i++)
                hc->m_frames[i] = luaFrames[2].cast<std::string>();
        } else {
            hc->m_frames[0] = luaFrames[luaFrames.length()].cast<std::string>();
            hc->m_frames[1] = luaFrames[luaFrames.length()].cast<std::string>();
            hc->m_frames[hc->m_maxHealth] = luaFrames[1].cast<std::string>();
            int interval = (hc->m_maxHealth - 2) / (luaFrames.length() - 2);
            int remainder = (hc->m_maxHealth - 2) % (luaFrames.length() - 2);
            int group = luaFrames.length() - 1;
            int counter = 0;
            for(int i = 2; i < hc->m_maxHealth; i++) {
                if(counter == interval + (remainder >= group)) {
                    group -= 1;
                    counter = 0;
                } else {
                    counter++;
                }
                hc->m_frames[i] = luaFrames[group].cast<std::string>();
            }
        }
        hc->m_deathFunc = std::make_shared<LuaRef>(luaDeathFunc);
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
        auto& gs = m_game.getWorld().getSystem<GraphicsSystem>();
        auto* gc = m_game.getWorld().getField()->getComponent<GraphicsComponent>(entity->getId());
        gs.setAnimation(gc, hc->m_frames[hc->m_health]);
    }

    void HealthSystem::update() {
        auto& gs = m_game.getWorld().getSystem<GraphicsSystem>();
        auto& hcList = m_game.getWorld().getField()->getComponentList<HealthComponent>()->m_list;
        for(auto& c : hcList) {
            auto& hc = *dynamic_cast<HealthComponent*>(c.second.get());
            hc.m_health -= hc.m_damage;
            hc.m_damage = 0;
            if(hc.m_health <= 0) {
                try {
                    if((*hc.m_deathFunc)(new LuaEntity(m_game, c.first)).cast<bool>()) {
                        m_game.getWorld().getField()->addDeleteEntity(c.first);
                    }
                } catch(luabridge::LuaException const& e) {
                    LogHandler::log<HealthSystem>(ERR, "LuaException: ");
                    std::cout << "                " << e.what() << std::endl;
                }
            } else {
                auto* gc = m_game.getWorld().getField()->getComponent<GraphicsComponent>(c.first);
                gs.setAnimation(gc, hc.m_frames[hc.m_health]);
            }
        }
    }

    void HealthSystem::addDamage(HealthComponent* hc, int damage) {
        hc->m_damage += damage;
    }
}