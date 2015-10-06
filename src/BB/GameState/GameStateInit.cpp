#include "BB/GameState/GameStateInit.h"
#include "BB/Game.h"
#include "BB/GameState/GameStateSplash.h"

#if defined (SFML_SYSTEM_WINDOWS)
#include <windows.h>

bool setShape(HWND hWnd, const sf::Image& image) {
    const sf::Uint8* pixelData = image.getPixelsPtr();
    HRGN hRegion = CreateRectRgn(0, 0, image.getSize().x, image.getSize().y);
    for(unsigned int y = 0; y < image.getSize().y; y++) {
        for(unsigned int x = 0; x < image.getSize().x; x++) {
            if(pixelData[y * image.getSize().x * 4 + x * 4 + 3] == 0) {
                HRGN hRegionDest = CreateRectRgn(0, 0, 1, 1);
                HRGN hRegionPixel = CreateRectRgn(x, y, x + 1, y + 1);
                CombineRgn(hRegionDest, hRegion, hRegionPixel, RGN_XOR);
                DeleteObject(hRegion);
                DeleteObject(hRegionPixel);
                hRegion = hRegionDest;
            }
        }
    }
    SetWindowRgn(hWnd, hRegion, true);
    DeleteObject(hRegion);
    return true;
}

bool setTransparency(HWND hWnd, unsigned char alpha) {
    SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
    SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
    return true;
}
#endif

namespace bb {
    GameStateInit::GameStateInit(Game& game):IGameState(game) {
        m_windowHandler = new WindowHandler();
        m_resourceHandler = new ResourceHandler(m_game);
        m_isRunning = true;
        using namespace luabridge;
        L = luaL_newstate();
        luaL_openlibs(L);
        m_resourceHandler->load(L);
        using namespace luabridge;
        if(luaL_loadfile(L, "assets/data/gameStateInit.lua") || lua_pcall(L, 0, 0, 0)) {
            LogHandler::log(LogHandler::ERR, "File \"assets/data/gameStateInit.lua\" not found",
                typeid(*this).name());
            return;
        }
        LuaRef luaTexture = getGlobal(L, "texture");
        if(luaTexture.isNil()) {
            LogHandler::log(LogHandler::ERR, "\"texture\" not found in gameStateInit.lua.",
                typeid(*this).name());
            return;
        }
        if(!luaTexture.isString()) {
            LogHandler::log(LogHandler::ERR, "\"texture\" not a string in gameStateInit.lua.",
                typeid(*this).name());
            return;
        }
        m_sprite.setTexture(m_resourceHandler->getTexture(luaTexture.cast<std::string>()));
        sf::Image init = m_resourceHandler->getTexture(luaTexture.cast<std::string>()).copyToImage();
        m_windowHandler->createWindow(sf::VideoMode(init.getSize().x, init.getSize().y, 32),
            "Brain Burst 2039", sf::Style::None, sf::ContextSettings());
        m_windowHandler->getWindow().setPosition(sf::Vector2i((sf::VideoMode::getDesktopMode().width - init.getSize().x) / 2, (sf::VideoMode::getDesktopMode().height - init.getSize().y) / 2));
        setShape(m_windowHandler->getWindow().getSystemHandle(), init);
        setTransparency(m_windowHandler->getWindow().getSystemHandle(), 200);
        m_updateCount = 0;
        m_finish = false;
    }

    GameStateInit::~GameStateInit() {
    }

    bool GameStateInit::update() {
        if(m_resourceHandler->load()) {
            if(!m_finish)
                LogHandler::log(LogHandler::INF, "Finished loading resources ", typeid(*this).name());
            m_isRunning = false;
            m_finish = true;
        }
        if(++m_updateCount >= 50) {
            if(!m_isRunning) {
                m_windowHandler->getWindow().close();
                m_game.changeState(new GameStateSplash(m_game, m_resourceHandler, L));
            }
        }
        return true;
    }

    void GameStateInit::draw(const double dt) {
        m_windowHandler->getWindow().clear(sf::Color::Transparent);
        m_windowHandler->getWindow().draw(m_sprite);
        m_windowHandler->getWindow().display();
    }

    void GameStateInit::handleInput() {
        sf::Event windowEvent;
        while(m_windowHandler->getWindow().pollEvent(windowEvent)) {
            if(windowEvent.type == sf::Event::Closed) {
                m_windowHandler->getWindow().close();
                m_isRunning = false;
                return;
            } else if(windowEvent.type == sf::Event::KeyPressed) {
                switch(windowEvent.key.code) {
                    case sf::Keyboard::Escape:
                        m_isRunning = false;
                        return;
                    case sf::Keyboard::Pause:
                        LogHandler::log(LogHandler::INF, "Breakpoint", typeid(*this).name());
                        break;
                }
            }
        }
    }
}