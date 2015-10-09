#include "BB/GameState/GameStateSplash.h"
#include "BB/Game.h"
#include "BB/GameState/GameStateTitle.h"

namespace bb {
    GameStateSplash::GameStateSplash(Game& game, ResourceHandler* resourceHandler,
        luabridge::lua_State* L):IGameState(game) {
        m_resourceHandler = resourceHandler;
        m_windowHandler = new WindowHandler();
        this->L = L;
        using namespace luabridge;
        if(luaL_loadfile(L, "assets/data/gameStateSplash.lua") || lua_pcall(L, 0, 0, 0)) {
            LogHandler::log(LogHandler::ERR, "File \"assets/data/gameStateSplash.lua\" not found",
                typeid(*this).name());
            return;
        }
        LuaRef luaSplashes = getGlobal(L, "splashes");
        LuaRef luaDuration = getGlobal(L, "duration");
        LuaRef luaWidth = getGlobal(L, "width");
        LuaRef luaHeight = getGlobal(L, "height");
        if(luaSplashes.isNil()) {
            LogHandler::log(LogHandler::ERR, "\"splashes\" not found in gameStateSplash.lua.",
                typeid(*this).name());
            return;
        }
        if(luaDuration.isNil()) {
            LogHandler::log(LogHandler::ERR, "\"duration\" not found in gameStateSplash.lua.",
                typeid(*this).name());
            return;
        }
        if(luaWidth.isNil()) {
            LogHandler::log(LogHandler::ERR, "\"width\" not found in gameStateSplash.lua.",
                typeid(*this).name());
            return;
        }
        if(luaHeight.isNil()) {
            LogHandler::log(LogHandler::ERR, "\"height\" not found in gameStateSplash.lua.",
                typeid(*this).name());
            return;
        }
        if(!luaSplashes.isTable()) {
            LogHandler::log(LogHandler::ERR, "\"splashes\" not a table in gameStateSplash.lua.",
                typeid(*this).name());
            return;
        }
        if(!luaDuration.isNumber()) {
            LogHandler::log(LogHandler::ERR, "\"duration\" not a integer in gameStateSplash.lua.",
                typeid(*this).name());
            return;
        }
        if(!luaWidth.isNumber()) {
            LogHandler::log(LogHandler::ERR, "\"width\" not a integer in gameStateSplash.lua.",
                typeid(*this).name());
            return;
        }
        if(!luaHeight.isNumber()) {
            LogHandler::log(LogHandler::ERR, "\"height\" not a integer in gameStateSplash.lua.",
                typeid(*this).name());
            return;
        }
        m_duration = luaDuration.cast<int>() * 50;
        m_splashes = 0;
        for(int i = 1; i <= luaSplashes.length(); i++) {
            LuaRef luaSplash = luaSplashes[i];
            LuaRef luaTexture = luaSplash["texture"];
            if(luaTexture.isNil()) {
                LogHandler::log(LogHandler::WRN, "\"texture\" not found in gameStateSplash.lua.",
                    typeid(*this).name());
                continue;
            }
            if(!luaTexture.isString()) {
                LogHandler::log(LogHandler::WRN, "\"texture\" of splash[" + std::to_string(i)
                    + "] not a string in gameStateSplash.lua.", typeid(*this).name());
                continue;
            }
            sf::Sprite sprite;
            sprite.setTexture(m_resourceHandler->getTexture(luaTexture.cast<std::string>()));
            sprite.setColor({255, 255, 255, 0});
            sprite.setScale(float(luaWidth.cast<float>() / 1920), float(luaHeight.cast<float>() / 1080));
            m_sprites.push_back(sprite);
            m_splashes++;
        }
        m_windowHandler->createWindow(sf::VideoMode(luaWidth.cast<int>(), luaHeight.cast<int>()),
            "Brain Burst 2039", sf::Style::Close);
        m_state = RUNNING;
        m_updateCount = int(m_duration * -0.5);
        m_splashCount = 0;
    }

    void GameStateSplash::handleInput() {
        sf::Event windowEvent;
        while(m_windowHandler->getWindow().pollEvent(windowEvent)) {
            if(windowEvent.type == sf::Event::Closed) {
                m_state = QUIT;
                return;
            } else if(windowEvent.type == sf::Event::KeyPressed) {
                switch(windowEvent.key.code) {
                    case sf::Keyboard::Escape:
                        m_state = QUIT;
                        return;
                }
            }
        }
    }

    bool GameStateSplash::update() {
        switch(m_state) {
            case RUNNING:
                if(m_splashes == 0) {
                    m_state = NEXT;
                } else {
                    m_updateCount++;
                    if(m_updateCount > 0) {
                        int c = m_updateCount - m_splashCount * m_duration;
                        if(c >= m_duration) {
                            m_splashCount++;
                            if(m_splashCount >= m_splashes)
                                m_state = NEXT;
                        } else {
                            int alpha = int(sin(double(c) / double(m_duration) * 3.14159265) * 255);
                            m_sprites[m_splashCount].setColor({255, 255, 255, sf::Uint8(alpha)});
                        }
                    }
                }
                break;
            case QUIT:
                m_windowHandler->getWindow().close();
                return false;
            case NEXT:
                m_game.changeState(new GameStateTitle(m_game, m_resourceHandler, m_windowHandler, L));
                break;
        }
        return true;
    }

    void GameStateSplash::draw(const double dt) {
        m_windowHandler->getWindow().clear({3, 3, 3, 255});
        if(m_splashes != 0 && m_state == RUNNING)
            m_windowHandler->getWindow().draw(m_sprites[m_splashCount]);
        m_windowHandler->getWindow().display();
    }
}