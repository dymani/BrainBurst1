#include "BB/Game.h"
#include "BB/GameState/GameStateInit.h"

int main() {
    bb::Game game;
    game.pushState(new bb::GameStateInit(game));
    return game.run();
}