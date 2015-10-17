#include "BB/World/Field.h"

namespace bb {
    Field::Field(ResourceHandler* resourceHandler, luabridge::lua_State* L, std::string world,
        sf::Vector2u windowSize, int id) {
        m_resourceHandler = resourceHandler;
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
        } else {
            LogHandler::log(LogHandler::ERR, "\"tileset\" not a table in " + file, typeid(*this).name());
        }
        LuaRef luaObjectTemplate = getGlobal(L, "object");
        if(luaObjectTemplate.isTable()) {
            sf::IntRect rect;
            sf::Vector2i size;
            for(int i = 1; i <= luaObjectTemplate.length(); i++) {
                LuaRef luaObject = luaObjectTemplate[i];
                LuaRef luaTextureRect = luaObject["textureRect"];
                LuaRef luaSize = luaObject["size"];
                rect = {luaTextureRect[1].cast<int>(), luaTextureRect[2].cast<int>(),
                    luaTextureRect[3].cast<int>(), luaTextureRect[4].cast<int>()};
                size = {luaSize[1].cast<int>(), luaSize[2].cast<int>()};
                m_objects[luaObject["name"].cast<std::string>()] = new Object(rect, size);
            }
        } else {
            LogHandler::log(LogHandler::ERR, "\"object\" not a table in " + file, typeid(*this).name());
        }
        LuaRef luaObjects = getGlobal(L, "objects");
        if(luaObjects.isTable()) {
            for(int i = 1; i <= luaObjects.length(); i++) {
                LuaRef luaObject = luaObjects[i];
                if(m_objects.find(luaObject["name"].cast<std::string>()) != m_objects.end()) {
                    Object* obj = new Object(*m_objects[luaObject["name"].cast<std::string>()]);
                    obj->setCoord({luaObject["coord"].cast<float>(), float(windowSize.y - 64)});
                    m_entities.push_back(obj);
                }
            }
        } else {
            LogHandler::log(LogHandler::ERR, "\"objects\" not a string in " + file, typeid(*this).name());
        }
        file = "saves/" + world + "/fields/" + idString(id) + ".lua";
        if(luaL_loadfile(L, file.c_str()) || lua_pcall(L, 0, 0, 0)) {
            LogHandler::log(LogHandler::ERR, "Field \"" + file + "\" not found", typeid(*this).name());
            return;
        }
        LuaRef luaType = getGlobal(L, "type");
        std::string type;
        if(luaType.isString()) {
            type = luaType.cast<std::string>();
        } else {
            type = "Default";
            LogHandler::log(LogHandler::ERR, "\"type\" not a string in " + file, typeid(*this).name());
        }
        file = "assets/data/world/stages/" + type + ".lua";
        if(luaL_loadfile(L, file.c_str()) || lua_pcall(L, 0, 0, 0)) {
            LogHandler::log(LogHandler::ERR, "Field \"" + file + "\" not found", typeid(*this).name());
            return;
        }
        LuaRef luaTileSet = getGlobal(L, "tileSet");
        if(luaTileSet.isString()) {
            m_tileSet = luaTileSet.cast<std::string>();
        } else {
            LogHandler::log(LogHandler::ERR, "\"tileset\" not a string in " + file, typeid(*this).name());
        }
        LuaRef luaObjectTexture = getGlobal(L, "objectTexture");
        if(luaObjectTexture.isString()) {
            m_objectTexture = luaObjectTexture.cast<std::string>();
        } else {
            LogHandler::log(LogHandler::ERR, "\"objectTexture\" not a string in " + file,
                typeid(*this).name());
        }
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(400);
        int height = windowSize.y - 64;
        for(int i = 0; i < 100; i++) {
            sf::Vertex* quad = &m_vertices[i * 4];
            quad[0].position = {float(i * 64), float(height)};
            quad[1].position = {float((i + 1) * 64), float(height)};
            quad[2].position = {float((i + 1) * 64), float(height + 64)};
            quad[3].position = {float(i * 64), float(height + 64)};
            quad[0].texCoords = {float(m_tiles[i] * 16), float(0)};
            quad[1].texCoords = {float(m_tiles[i] * 16 + 16), float(0)};
            quad[2].texCoords = {float(m_tiles[i] * 16 + 16), float(16)};
            quad[3].texCoords = {float(m_tiles[i] * 16), float(16)};
        }
    }

    void Field::update() {

    }

    void Field::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform = getTransform();
        sf::Texture& tex = m_resourceHandler->getTexture(m_tileSet);
        tex.setSmooth(false);
        states.texture = &tex;
        target.draw(m_vertices, states);
        sf::Texture& obj = m_resourceHandler->getTexture(m_objectTexture);
        for(auto& entity : m_entities) {
            entity->draw(target, obj);
        }
    }

    std::string Field::idString(int id) {
        if(id >= 10)
            return std::to_string(id);
        else
            return "0" + std::to_string(id);
    }
}