#include "game.hpp"

int main() {
  Game *game = new Game();

  int field[7][7] = {
     { -1, -1, 1, 1, 1, -1, -1 },
     { -1, -1, 1, 1, 1, -1, -1 },
     {  1,  1, 1, 1, 1,  1,  1 },
     {  1,  1, 1, 0, 1,  1,  1 },
     {  1,  1, 1, 1, 1,  1,  1 },
     { -1, -1, 1, 1, 1, -1, -1 },
     { -1, -1, 1, 1, 1, -1, -1 }
  };

  game->initialize((int**) &field[0], sf::Vector2u(7, 7), sf::Vector2u(3, 3));
  game->run();

  return 0;
}
