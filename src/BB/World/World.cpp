#include "BB/World/World.h"
#include "BB/GameState/GameStateGame.h"

namespace bb {
    World::World(GameStateGame& game, std::string name) : m_game(game){
        m_name = name;
        addSystem(new GraphicsSystem(m_game));
        addSystem(new PhysicsSystem(m_game));
        addSystem(new ControlSystem(m_game));
        auto* L = m_game.getLuaState();
        using namespace luabridge;
        std::string file = "assets/data/world/world.lua";
        if(luaL_loadfile(L, file.c_str()) || lua_pcall(L, 0, 0, 0)) {
            LogHandler::log<Field>(ERR, "World \"" + file + "\" not found");
            assert(false);
            return;
        }
        luabridge::LuaRef& luaEntities = getGlobal(L, "entities");
        if(luaEntities.isTable()) {
            for(int i = 1; i <= luaEntities.length(); i++) {
                luabridge::LuaRef luaEntity = luaEntities[i];
                EntityTemplate* et = new EntityTemplate(m_game, luaEntity);
                m_entityTemplates[et->getName()] = et;
            }
        }
        m_field = new Field(m_game, m_name, "01");
        m_field->load();
    }

    void World::handleInput() {
        getSystem<ControlSystem>().handleInput();
    }

    void World::handleInput(sf::Event& windowEvent) {
        getSystem<ControlSystem>().handleInput(windowEvent);
        if(windowEvent.type == sf::Event::KeyPressed) {
            auto coord = getSystem<GraphicsSystem>().getViewCoord();
            if(windowEvent.key.code == sf::Keyboard::Left)
                getSystem<GraphicsSystem>().setViewCoord(coord.x - 0.1, coord.y);
            else if(windowEvent.key.code == sf::Keyboard::Right)
                getSystem<GraphicsSystem>().setViewCoord(coord.x + 0.1, coord.y);
            else if(windowEvent.key.code == sf::Keyboard::Up)
                getSystem<GraphicsSystem>().setViewCoord(coord.x, coord.y + 0.1);
            else if(windowEvent.key.code == sf::Keyboard::Down)
                getSystem<GraphicsSystem>().setViewCoord(coord.x, coord.y - 0.1);
        }
    }

    void World::update() {
        m_field->update();
    }

    void World::draw(const double dt) {
        m_field->draw(dt);
    }

    Field * World::getField() {
        return m_field;
    }

    Stage* World::getStage(std::string name) {
        if(m_stages.find(name) != m_stages.end())
            return m_stages[name];
        Stage* stage = Stage::create(m_game.getLuaState(), name);
        if(stage)
            m_stages[name] = stage;
        return stage;
    }

    EntityTemplate* World::getEntityTemplate(std::string name) {
        return m_entityTemplates[name];
    }

    std::map<std::type_index, ISystem*>& World::getSystems() {
        return m_systems;
    }
}