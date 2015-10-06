#include "BB/GameState/GameStateSplash.h"
#include "BB/Game.h"

namespace bb {
    GameStateSplash::GameStateSplash(Game& game, ResourceHandler* resourceHandler,
        luabridge::lua_State* L):IGameState(game) {
        m_resourceHandler = resourceHandler;
        m_windowHandler = new WindowHandler();
        this->L = L;
        m_windowHandler->createWindow(sf::VideoMode(1024, 576), "Brain Burst 2039", sf::Style::Close);
        using namespace luabridge;
        if(luaL_loadfile(L, "assets/data/gameStateSplash.lua") || lua_pcall(L, 0, 0, 0)) {
            LogHandler::log(LogHandler::ERR, "File \"assets/data/gameStateSplash.lua\" not found",
                typeid(*this).name());
            return;
        }
        LuaRef luaSplashes = getGlobal(L, "splashes");
        LuaRef luaDuration = getGlobal(L, "duration");
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
        m_duration = luaDuration.cast<int>() * 50;
        m_splashes = 0;
        int k = luaSplashes.length();
        for(int i = 1; i <= luaSplashes.length(); i++) {
            LuaRef luaSplash = luaSplashes[i];
            LuaRef luaTexture = luaSplash["texture"];
            LuaRef luaScale = luaSplash["scale"];
            if(luaTexture.isNil()) {
                LogHandler::log(LogHandler::WRN, "\"texture\" not found in gameStateSplash.lua.",
                    typeid(*this).name());
                continue;
            }
            if(luaScale.isNil()) {
                LogHandler::log(LogHandler::WRN, "\"scale\" not found in gameStateSplash.lua.",
                    typeid(*this).name());
                continue;
            }
            if(!luaTexture.isString()) {
                LogHandler::log(LogHandler::WRN, "\"texture\" of splash[" + std::to_string(i)
                    + "] not a string in gameStateSplash.lua.", typeid(*this).name());
                continue;
            }
            if(!luaScale.isNumber()) {
                LogHandler::log(LogHandler::WRN, "\"scale\" of splash[" + std::to_string(i)
                    + "] not a number in gameStateSplash.lua.", typeid(*this).name());
                continue;
            }
            sf::Sprite sprite;
            sprite.setTexture(m_resourceHandler->getTexture(luaTexture.cast<std::string>()));
            sprite.setScale(luaScale.cast<float>(), luaScale.cast<float>());
            sprite.setColor({255, 255, 255, 0});
            m_sprites.push_back(sprite);
            m_splashes++;
        }
        m_isRunning = true;
        m_updateCount = int(m_duration * -0.5);
        m_splashCount = 0;
    }

    bool GameStateSplash::update() {
        if(m_splashes == 0) {
            m_isRunning = false;
        } else {
            m_updateCount++;
            if(m_updateCount > 0) {
                int c = m_updateCount - m_splashCount * m_duration;
                if(c >= m_duration) {
                    m_splashCount++;
                    if(m_splashCount >= m_splashes)
                        m_isRunning = false;
                } else {
                    int alpha = int(sin(double(c) / double(m_duration) * 3.14159265) * 255);
                    m_sprites[m_splashCount].setColor({255, 255, 255, sf::Uint8(alpha)});
                }
            }
        }
        if(!m_isRunning) {
            m_windowHandler->getWindow().close();
        }
        return m_isRunning;
    }

    void GameStateSplash::draw(const double dt) {
        m_windowHandler->getWindow().clear(sf::Color::Black);
        if(m_splashes != 0 && m_isRunning)
            m_windowHandler->getWindow().draw(m_sprites[m_splashCount]);
        m_windowHandler->getWindow().display();
    }

    void GameStateSplash::handleInput() {
        sf::Event windowEvent;
        while(m_windowHandler->getWindow().pollEvent(windowEvent)) {
            if(windowEvent.type == sf::Event::Closed) {
                m_windowHandler->getWindow().close();
                m_isRunning = false;
                return;
            } else if(windowEvent.type == sf::Event::KeyPressed) {
                switch(windowEvent.key.code) {
                    case sf::Keyboard::Escape:
                        m_windowHandler->getWindow().close();
                        m_isRunning = false;
                        return;
                    case sf::Keyboard::F5:
                        m_game.changeState(new GameStateSplash(m_game, m_resourceHandler, L));
                        LogHandler::log(LogHandler::INF, "Restarting splash state", typeid(*this).name());
                        break;
                    case sf::Keyboard::Pause:
                        LogHandler::log(LogHandler::INF, "Breakpoint", typeid(*this).name());
                        break;
                }
            }
        }
    }
}