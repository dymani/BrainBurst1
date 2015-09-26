#ifndef AUDIO_HANDLER_H
#define AUDIO_HANDLER_H

#include <SFML/Audio.hpp>
#include <map>

namespace bb {
    class ResourceHandler;

    class AudioHandler {
    public:
        AudioHandler(ResourceHandler& resourceHandler);
        ~AudioHandler();
        void update();
        int play(std::string name, float volume = 100.0F, bool loop = false);
        void togglePause(int id);
        void stop(int id);
    private:
        ResourceHandler& m_resourcehandler;
        std::map<int, sf::Sound*> m_sounds;
        int id = 0;
    };
}

#endif