#include "BB/World/World.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    World::World(GameStateGame& game, std::string name, luabridge::lua_State* L) : m_game(game) {
        m_entities.clear();
        m_name = name;
        this->L = L;
        using namespace luabridge;
        std::string file = "assets/data/world/world.lua";
        if(luaL_loadfile(L, file.c_str()) || lua_pcall(L, 0, 0, 0)) {
            LogHandler::log(LogHandler::ERR, "World \"" + file + "\" not found", typeid(*this).name());
            return;
        }
        LuaRef luaPlayer = getGlobal(L, "player");
        Entity* player = Entity::create(m_game, L, luaPlayer);
        file = "saves/" + m_name + "/world.json";
        std::ifstream fin(file);
        if(fin.fail()) {
            LogHandler::log(LogHandler::ERR, "World \"" + file + "\" not found", typeid(*this).name());
            return;
        }
        std::stringstream strStream;
        strStream << fin.rdbuf();
        std::string fileString = strStream.str();
        fin.close();
        using namespace rapidjson;
        Document document;
        if(document.Parse<0>(fileString.c_str()).HasParseError()) {
            LogHandler::log(LogHandler::ERR, "Failed to parse \"" + file + "\"", typeid(*this).name());
            return;
        }
        Value& jsonPlayer = document["player"];
        Entity* entity = new Entity(*player, jsonPlayer);
        m_entities[entity->getId()] = entity;
        m_fieldId = jsonPlayer["field"].GetString();
    }

    void World::createField() {
        m_field = new Field(m_game, L, m_fieldId);
        m_field->load(m_name);
    }

    void World::handleInput() {
        m_entities[0]->getComponent<PlayerComponent>()->handleInput();
    }

    void World::handleInput(sf::Event windowEvent) {

    }

    void World::update() {
        m_game.getPhysicsHandler()->update(m_entities);
    }

    void World::draw() {
        m_field->draw();
        for(auto& entity : m_entities) {
            m_game.getGraphicsHandler()->draw(entity.first);
        }
    }

    Stage* World::getStage(std::string name) {
        if(m_stages.find(name) != m_stages.end())
            return m_stages[name];
        Stage* stage = Stage::create(L, name);
        if(stage)
            m_stages[name] = stage;
        return stage;
    }

    Entity* World::getEntity(int id) {
        if(m_entities.find(id) != m_entities.end())
            return m_entities[id];
        return nullptr;
    }

    std::map<int, Entity*>& World::getEntities() {
        return m_entities;
    }
}