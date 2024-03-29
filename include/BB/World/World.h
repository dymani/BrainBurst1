#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>
#include <LuaBridge\LuaBridge.h>
extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}
#include <rapidjson/document.h>
#include "BB/World/EntityTemplate.h"
#include "BB/World/Field.h"
#include "BB/World/Stage.h"
#include "BB/World/System/ISystem.h"
#include "BB/World/System/GraphicsSystem.h"
#include "BB/World/System/PhysicsSystem.h"
#include "BB/World/System/ControlSystem.h"
#include "BB/World/System/HealthSystem.h"

namespace bb {
    class GameStateGame;

    class World {
    public:
        World(GameStateGame& game, std::string name);
        void handleInput();
        void handleInput(sf::Event& windowEvent);
        void update();
        void draw(const double dt);
        Field* getField();
        Stage* getStage(std::string name);
        EntityTemplate* getEntityTemplate(std::string name);
        std::map<std::type_index, std::unique_ptr<ISystem>>& getSystems();
        template<typename T>
        T& getSystem() {
            return *dynamic_cast<T*>(m_systems[std::type_index(typeid(T))].get());
        }
    private:
        template<typename T>
        void addSystem(T* system) {
            m_systems[std::type_index(typeid(T))] = std::unique_ptr<ISystem>(system);
        }
        GameStateGame& m_game;
        std::string m_name, m_fieldId;
        std::unique_ptr<Field> m_field;
        std::map<std::string, std::unique_ptr<Stage>> m_stages;
        std::map<std::string, std::unique_ptr<EntityTemplate>> m_entityTemplates;
        std::map<std::type_index, std::unique_ptr<ISystem>> m_systems;
    };
}

#endif