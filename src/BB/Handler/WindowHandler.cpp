#include "BB/Handler/WindowHandler.h"

namespace bb {
    WindowHandler::WindowHandler() {
    }

    void WindowHandler::createWindow(sf::VideoMode videoMode, const std::string& title, sf::Uint32 style,
        const sf::ContextSettings& settings) {
        m_window.create(videoMode, title, style, settings);
        m_isCreated = true;
    }

    sf::RenderWindow& WindowHandler::getWindow() {
        if(!m_isCreated) {
            LogHandler::log(LogHandler::ERR, "Window not created", typeid(*this).name());
            createWindow(sf::VideoMode(100, 100), "ERROR");
        }
        return m_window;
    }
}