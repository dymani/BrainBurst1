#include "BB/World/Field.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/World/Component/GraphicsComponent.h"
#include "BB/World/Component/MovementComponent.h"

namespace bb {
    Field::Field(GameStateGame& game, std::string worldName, std::string id) : m_game(game) {
        for(auto& system : m_game.getWorld().getSystems()) {
            system.second->createList(m_componentLists);
        }
        m_worldName = worldName;
        m_id = id;
    }

    void Field::load() {
        std::string file = "saves/" + m_worldName + "/fields/" + m_id + ".json";
        std::ifstream fin(file);
        if(fin.fail()) {
            LogHandler::log<Field>(ERR, "Field \"" + file + "\" not found");
            return;
        }
        std::stringstream strStream;
        strStream << fin.rdbuf();
        std::string fileString = strStream.str();
        fin.close();
        using namespace rapidjson;
        Document document;
        if(document.Parse<0>(fileString.c_str()).HasParseError()) {
            LogHandler::log<Field>(ERR, "Failed to parse \"" + file + "\"");
            return;
        }
        Value& jsonEntities = document["entities"];
        for(SizeType i = 0; i < jsonEntities.Size(); i++) {
            Value& jsonEntity = jsonEntities[i];
            Value& jsonName = jsonEntity["name"];
            Entity* entity = m_game.getWorld().getEntityTemplate(jsonName.GetString())->createEntity(m_game,
                jsonEntity);
            m_entities[entity->getId()] = entity;
        }
        m_tileSet = m_game.getWorld().getStage(document["tileSet"].GetString())->getTileSet();
        Value& jsonTiles = document["tiles"];
        for(SizeType i = 0; i < jsonTiles.Size(); i++) {
            m_tiles.push_back(jsonTiles[i].GetInt());
        }
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(int(m_tiles.size()) * 4);
        int textureSize = m_game.getWorld().getSystem<GraphicsSystem>().getTextureSize();
        float tileSize = m_game.getWorld().getSystem<GraphicsSystem>().getTileSize();
        int h = m_game.getWindowHandler()->getWindow().getSize().y;
        for(int i = 0; i < int(m_tiles.size()); i++) {
            sf::Vertex* quad = &m_vertices[i * 4];
            quad[0].position = {float(i * tileSize), float(h)};
            quad[1].position = {float((i + 1) * tileSize), float(h)};
            quad[2].position = {float((i + 1) * tileSize), float(h + tileSize)};
            quad[3].position = {float(i * tileSize), float(h + tileSize)};
            quad[0].texCoords = {float(m_tiles[i] * textureSize), float(0)};
            quad[1].texCoords = {float(m_tiles[i] + 1) * textureSize, float(0)};
            quad[2].texCoords = {float(m_tiles[i] + 1) * textureSize, float(textureSize)};
            quad[3].texCoords = {float(m_tiles[i] * textureSize), float(textureSize)};
        }
        m_background.setTexture(m_game.getResourceHandler()->getTexture(m_game.getWorld()
            .getStage(document["tileSet"].GetString())->getBackgroundTexture()));
        sf::Texture& tex = m_game.getResourceHandler()->getTexture(m_tileSet);
        tex.setSmooth(false);
        m_states.texture = &tex;
        sf::View view = m_game.getWindowHandler()->getWindow().getDefaultView();
        m_background.setScale(view.getSize().x / m_background.getTexture()->getSize().x,
            view.getSize().x / m_background.getTexture()->getSize().x);
        m_game.getWorld().getSystem<GraphicsSystem>().setViewCoord(0, -1);
        m_playerId = m_componentLists[std::type_index(typeid(PlayerComponent))]->begin()->first;
    }

    void Field::update() {
    }

    void Field::draw(const double dt) {
        auto gs = m_game.getWorld().getSystem<GraphicsSystem>();
        sf::Vector2f playerCoord = m_entities[m_playerId]->getCoord();
        if(gs.getViewCoord().x > playerCoord.x + 3) {
            gs.setViewCoord(playerCoord.x + 3, gs.getViewCoord().y);
        } else if(gs.getViewCoord().x < playerCoord.x - 3) {
            gs.setViewCoord(playerCoord.x - 3, gs.getViewCoord().y);
        }
        if(gs.getViewCoord().y < playerCoord.y - 5) {
            gs.setViewCoord(gs.getViewCoord().x, playerCoord.y - 5);
        } else if(gs.getViewCoord().y > playerCoord.y - 3) {
            gs.setViewCoord(gs.getViewCoord().x, playerCoord.y - 3);
        }
        sf::View view = gs.getView();
        auto& window = m_game.getWindowHandler()->getWindow();
        window.setView(window.getDefaultView());
        window.draw(m_background);
        window.setView(view);
        window.draw(m_vertices, m_states);
        gs.draw(dt);
    }

    int Field::getSize() {
        return int(m_tiles.size());
    }

    Entity* Field::getEntity(int id) {
        return m_entities[id];
    }

    void Field::addDeleteEntity(int id) {
        m_deletingEntities.push_back(id);
    }

    void Field::deleteEntities() {
        for(int id : m_deletingEntities) {
            delete m_entities[id];
            m_entities.erase(id);
        }
        m_deletingEntities.clear();
    }

    void Field::deleteComponents() {
        for(auto& c : m_deletingComponents) {
            m_entities[c.second]->removeComponent(c.first);
        }
        m_deletingEntities.clear();
    }

    std::map<int, IComponent*>* Field::getComponentList(std::type_index type) {
        return m_componentLists[type];
    }
}