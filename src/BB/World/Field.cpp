#include "BB/World/Field.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/GraphicsHandler.h"
#include "BB/Handler/MovementHandler.h"

namespace bb {
    Field::Field(GameStateGame& game, luabridge::lua_State* L, std::string world, int height, int id)
    :m_game(game){
        this->L = L;
        using namespace luabridge;
        std::string file = "assets/data/world/fields/" + idString(id) + ".lua";
        if(luaL_loadfile(L, file.c_str()) || lua_pcall(L, 0, 0, 0)) {
            LogHandler::log(LogHandler::ERR, "Field \"" + file + "\" not found", typeid(*this).name());
            return;
        }
        LuaRef luaTiles = getGlobal(L, "tiles");
        if(luaTiles.isTable()) {
            for(int i = 1; i <= luaTiles.length() && i <= 100; i++) {
                m_tiles[i - 1] = luaTiles[i].cast<int>();
            }
        }
        LuaRef luaObjectTemplate = getGlobal(L, "object");
        if(luaObjectTemplate.isTable()) {
            sf::IntRect rect;
            sf::Vector2i size;
            for(int i = 1; i <= luaObjectTemplate.length(); i++) {
                LuaRef luaObject = luaObjectTemplate[i];
                LuaRef luaName = luaObject["name"];
                m_objects[luaName.cast<std::string>()] = Entity::create(m_game, -1, L, luaObject);
            }
        }
        LuaRef luaObjects = getGlobal(L, "objects");
        if(luaObjects.isTable()) {
            for(int i = 1; i <= luaObjects.length(); i++) {
                LuaRef luaObject = luaObjects[i];
                if(m_objects.find(luaObject["name"].cast<std::string>()) != m_objects.end()) {
                    Entity* entity = new Entity(*m_objects[luaObject["name"].cast<std::string>()],
                        m_game.getEntities().size());
                    entity->setCoord({luaObject["coord"].cast<float>(), 0});
                    m_game.getEntities().push_back(entity);
                }
            }
        }
        file = "saves/" + world + "/fields/" + idString(id) + ".lua";
        if(luaL_loadfile(L, file.c_str()) || lua_pcall(L, 0, 0, 0)) {
            LogHandler::log(LogHandler::ERR, "Field \"" + file + "\" not found", typeid(*this).name());
            return;
        }
        LuaRef luaType = getGlobal(L, "type");
        LuaRef luaEntities = getGlobal(L, "entities");
        std::string type;
        if(luaType.isString()) {
            type = luaType.cast<std::string>();
        }
        if(luaEntities.isTable()) {
            for(int i = 1; i <= luaEntities.length(); i++) {
                LuaRef luaEntity = luaEntities[i];
                Entity* entity = Entity::create(m_game, m_game.getEntities().size(), L, luaEntity);
                m_game.getEntities().push_back(entity);
            }
        }
        file = "assets/data/world/stages/" + type + ".lua";
        if(luaL_loadfile(L, file.c_str()) || lua_pcall(L, 0, 0, 0)) {
            LogHandler::log(LogHandler::ERR, "Field \"" + file + "\" not found", typeid(*this).name());
            return;
        }
        LuaRef luaTileSet = getGlobal(L, "tileSet");
        if(luaTileSet.isString()) {
            m_tileSet = luaTileSet.cast<std::string>();
        }
        LuaRef luaObjectTexture = getGlobal(L, "objectTexture");
        if(luaObjectTexture.isString()) {
            m_objectTexture = luaObjectTexture.cast<std::string>();
        }
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(400);
        int h = height - 64;
        for(int i = 0; i < 100; i++) {
            sf::Vertex* quad = &m_vertices[i * 4];
            quad[0].position = {float(i * 64), float(h)};
            quad[1].position = {float((i + 1) * 64), float(h)};
            quad[2].position = {float((i + 1) * 64), float(h + 64)};
            quad[3].position = {float(i * 64), float(h + 64)};
            quad[0].texCoords = {float(m_tiles[i] * 16), float(0)};
            quad[1].texCoords = {float(m_tiles[i] * 16 + 16), float(0)};
            quad[2].texCoords = {float(m_tiles[i] * 16 + 16), float(16)};
            quad[3].texCoords = {float(m_tiles[i] * 16), float(16)};
        }
        sf::Texture& obj = m_game.getResourceHandler()->getTexture(m_objectTexture);
        obj.setSmooth(false);
        for(auto& entity : m_game.getEntities()) {
            for(auto& sprite : entity->getComponent<GraphicsComponent>()->getSprites()) {
                sprite.second->setTexture(obj);
            }
        }
    }

    void Field::draw() {
        sf::Texture& tex = m_game.getResourceHandler()->getTexture(m_tileSet);
        tex.setSmooth(false);
        m_states.texture = &tex;
        m_game.getWindowHandler()->getWindow().draw(m_vertices, m_states);
    }

    std::string Field::idString(int id) {
        if(id >= 10)
            return std::to_string(id);
        else
            return "0" + std::to_string(id);
    }
}