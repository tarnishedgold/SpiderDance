#include "Game.h"
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    Game game;
    if (!game.run()) {
        return 1;
    }
    return 0;
}