#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

#include "field.hpp"
#include "renderer.hpp"

#define START_ROW 3
#define START_COL 3

class Game {
public:
  Game();

  ~Game();

  void initialize(FieldElement **fieldData, sf::Vector2u fieldSize, sf::Vector2u startPos);

  void run();

protected:
  bool handleEvents();

  void draw();

private:
  Field *field;
  Renderer *renderer;

  sf::Vector2i cursorPos;
  sf::Vector2i selectionPos;
};
