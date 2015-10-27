#include "BB/World/World.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    World::World(GameStateGame& game, std::string name, luabridge::lua_State* L) : m_game(game) {
        m_entities.clear();
        m_name = name;
        this->L = L;
        using namespace luabridge;
        std::string file = "saves/" + m_name + "/world.lua";
        if(luaL_loadfile(L, file.c_str()) || lua_pcall(L, 0, 0, 0)) {
            LogHandler::log(LogHandler::ERR, "World \"" + file + "\" not found", typeid(*this).name());
            return;
        }
        LuaRef luaPlayer = getGlobal(L, "player");
        if(luaPlayer.isTable()) {
            Entity* player = Entity::create(m_game, 0, L, luaPlayer);
            m_entities.push_back(player);
            LuaRef luaFieldId = luaPlayer["field"];
            if(luaFieldId.isString()) {
                m_fieldId = luaFieldId.cast<std::string>();
            }
        }
    }

    void World::createField() {
        using namespace luabridge;
        std::string file = "saves/" + m_name + "/fields/" + m_fieldId + ".lua";
        if(luaL_loadfile(L, file.c_str()) || lua_pcall(L, 0, 0, 0)) {
            LogHandler::log(LogHandler::ERR, "Field \"" + file + "\" not found", typeid(*this).name());
            return;
        }
        m_field = new Field(m_game, L, m_fieldId);
    }

    void World::handleInput() {
        m_entities[0]->getComponent<PlayerComponent>()->handleInput();
    }

    void World::handleInput(sf::Event windowEvent) {

    }

    void World::update() {
        m_game.getMovementHandler()->update(m_entities);
    }

    void World::draw() {
        m_field->draw();
        for(unsigned int i = 0; i < m_entities.size(); i++) {
            m_game.getGraphicsHandler()->draw(int(i));
        }
    }

    Entity* World::getEntity(int id) {
        if(m_entities[id])
            return m_entities[id];
        return nullptr;
    }

    std::vector<Entity*>& World::getEntities() {
        return m_entities;
    }
}