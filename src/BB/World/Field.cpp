#include "BB/World/Field.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/Component/GraphicsComponent.h"
#include "BB/Handler/ResourceHandler.h"
#include "BB/Handler/GraphicsHandler.h"
#include "BB/Handler/PhysicsHandler.h"
#include "BB/World/Stage.h"

namespace bb {
    Field::Field(GameStateGame& game, luabridge::lua_State* L, std::string id) : m_game(game) {
        this->L = L;
        m_id = id;
        using namespace luabridge;
        std::string file = "assets/data/world/fields/" + m_id + ".lua";
        if(luaL_loadfile(L, file.c_str()) || lua_pcall(L, 0, 0, 0)) {
            LogHandler::log(LogHandler::ERR, "Field \"" + file + "\" not found", typeid(*this).name());
            return;
        }
        LuaRef luaObjects = getGlobal(L, "object");
        if(luaObjects.isTable()) {
            sf::IntRect rect;
            sf::Vector2i size;
            for(int i = 1; i <= luaObjects.length(); i++) {
                LuaRef luaObject = luaObjects[i];
                m_objectsTemplate[luaObject["name"].cast<std::string>()] = Entity::create(m_game, L,
                    luaObject);
            }
        }
        LuaRef luaEntities = getGlobal(L, "entities");
        if(luaEntities.isTable()) {
            for(int i = 1; i <= luaEntities.length(); i++) {
                LuaRef luaEntity = luaEntities[i];
                m_entitiesTemplate[luaEntity["name"].cast<std::string>()] = Entity::create(m_game, L,
                    luaEntity);
            }
        }
    }

    void Field::load(std::string worldName) {
        std::string file = "saves/" + worldName + "/fields/" + m_id + ".json";
        std::ifstream fin(file);
        if(fin.fail()) {
            LogHandler::log(LogHandler::ERR, "Field \"" + file + "\" not found", typeid(*this).name());
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
        Value& jsonObjects = document["objects"];
        for(SizeType i = 0; i < jsonObjects.Size(); i++) {
            Value& jsonObject = jsonObjects[i];
            Entity* object = new Entity(*m_objectsTemplate[jsonObject["name"].GetString()], jsonObject);
            m_game.getWorld()->getEntities()[object->getId()] = object;
            m_objects.push_back(object->getId());
        }
        Value& jsonEntities = document["entities"];
        for(SizeType i = 0; i < jsonEntities.Size(); i++) {
            Value& jsonEntity = jsonEntities[i];
            Entity* entity = new Entity(*m_entitiesTemplate[jsonEntity["name"].GetString()], jsonEntity);
            m_game.getWorld()->getEntities()[entity->getId()] = entity;
            m_entities.push_back(entity->getId());
        }
        m_tileSet = m_game.getWorld()->getStage(document["tileSet"].GetString())->getTileSet();
        Value& jsonTiles = document["tiles"];
        for(SizeType i = 0; i < jsonTiles.Size(); i++) {
            m_tiles.push_back(jsonTiles[i].GetInt());
        }
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(int(m_tiles.size()) * 4);
        int h = m_game.getWindowHandler()->getWindow().getSize().y - 64;
        for(int i = 0; i < int(m_tiles.size()); i++) {
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
        m_background.setTexture(m_game.getResourceHandler()->getTexture(m_game.getWorld()->getStage(document["tileSet"].GetString())->getBackgroundTexture()));
    }

    void Field::draw() {
        sf::View view = m_game.getWindowHandler()->getWindow().getView();
        m_game.getWindowHandler()->getWindow().setView(m_game.getWindowHandler()->getWindow().getDefaultView());
        m_background.setScale(view.getSize().x / m_background.getTexture()->getSize().x,
            view.getSize().x / m_background.getTexture()->getSize().x);
        m_game.getWindowHandler()->getWindow().draw(m_background);
        m_game.getWindowHandler()->getWindow().setView(view);
        sf::Texture& tex = m_game.getResourceHandler()->getTexture(m_tileSet);
        tex.setSmooth(false);
        m_states.texture = &tex;
        m_game.getWindowHandler()->getWindow().draw(m_vertices, m_states);
    }

    int Field::getSize() {
        return int(m_tiles.size());
    }
}