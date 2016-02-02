#include "BB/GameState/GameStateTitle.h"
#include "BB/Game.h"
#include "BB/GameState/GameStateGame.h"
#include "BB/GameState/GameStateInit.h"
#include "BB/Handler/Gui/Button.h"

namespace bb {
    GameStateTitle::GameStateTitle(Game& game, ResourceHandler* resourceHandler,
        WindowHandler* windowHandler) : IGameState(game),
        m_resourceHandler(std::unique_ptr<ResourceHandler>(resourceHandler)),
        m_windowHandler(std::unique_ptr<WindowHandler>(windowHandler)) {
        m_windowHandler->getWindow().setView(m_windowHandler->getWindow().getDefaultView());
        m_guiHandler = std::unique_ptr<GuiHandler>(new GuiHandler(*m_windowHandler.get(),
            *m_resourceHandler.get()));
        using namespace luabridge;
        L = luaL_newstate();
        luaL_openlibs(L);
        if(luaL_loadfile(L, "assets/data/gameStates/title.lua") || lua_pcall(L, 0, 0, 0)) {
            LogHandler::log<GameStateTitle>(ERR, "File \"assets/data/gameStates/title.lua\" not found");
            assert(false);
            return;
        }
        LuaRef luaBackground = getGlobal(L, "background");
        m_background.setTexture(m_resourceHandler->getTexture(luaBackground.cast<std::string>()));
        m_background.setScale(
            float(m_windowHandler->getWindow().getSize().x) / float(m_background.getTexture()->getSize().x),
            float(m_windowHandler->getWindow().getSize().y) / float(m_background.getTexture()->getSize().y));
        LuaRef luaCallbacks = getGlobal(L, "callbacks");
        for(int i = 1; i <= luaCallbacks.length(); i++) {
            LuaRef luaCallback = luaCallbacks[i];
            m_callbacks[luaCallback["id"].cast<int>()] = luaCallback["state"].cast<int>();
        }
        LuaRef luaGui = getGlobal(L, "gui");
        m_guiHandler->load(luaGui);
        m_state = RUNNING;
    }

    void GameStateTitle::handleInput() {
        sf::Event windowEvent;
        while(m_windowHandler->getWindow().pollEvent(windowEvent)) {
            m_guiHandler->handleInput(windowEvent);
            if(windowEvent.type == sf::Event::Closed) {
                m_state = QUIT;
                return;
            } else if(windowEvent.type == sf::Event::KeyPressed) {
                switch(windowEvent.key.code) {
                    case sf::Keyboard::Return:
                    case sf::Keyboard::Space:
                        m_state = GAME;
                        break;
                    case sf::Keyboard::Escape:
                        m_state = QUIT;
                        return;
                    case sf::Keyboard::F5:
                        m_state = RELOAD;
                        break;
                }
            }
        }
    }

    bool GameStateTitle::update() {
        int i = m_guiHandler->update();
        if(i != -1){
            m_state = State(m_callbacks[i]);
        }
        switch(m_state) {
            case RUNNING:
                break;
            case QUIT:
                m_windowHandler->getWindow().close();
                return false;
            case GAME:
                m_game.changeState(new GameStateGame(m_game, m_resourceHandler.release(),
                    m_windowHandler.release()));
                break;
            case RELOAD:
                m_windowHandler->getWindow().close();
                m_game.changeState(new GameStateInit(m_game));
                break;
        }
        return true;
    }

    void GameStateTitle::draw(const double dt) {
        m_windowHandler->getWindow().clear(sf::Color::Black);
        m_windowHandler->getWindow().draw(m_background);
        m_guiHandler->draw();
        m_windowHandler->getWindow().display();
    }
}