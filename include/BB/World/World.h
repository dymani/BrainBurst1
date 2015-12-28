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
#include "BB/World/Field.h"
#include "BB/World/Stage.h"
#include "BB/World/System/GraphicsSystem.h"
#include "BB/World/System/PhysicsSystem.h"

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
        GraphicsSystem& getGraphicsSystem();
        PhysicsSystem& getPhysicsSystem();
    private:
        GameStateGame& m_game;
        std::string m_name, m_fieldId;
        Field* m_field;
        std::map<std::string, Stage*> m_stages;
        GraphicsSystem m_graphicsSystem;
        PhysicsSystem m_physicsSystem;
    };
}

#endif