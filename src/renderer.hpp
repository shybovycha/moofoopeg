#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <cstdlib>
#include <iostream>
#include <string>
#include <memory>

#include "field.hpp"

class Renderer {
public:
  Renderer();

  ~Renderer();

  void initialize();

  void draw(std::shared_ptr<Field> field, sf::Vector2i cursorPos, sf::Vector2i selectionPos);

  sf::Event* pollEvent();

protected:
  sf::Sprite* loadSpriteFromFile(const char *path);

  sf::Font* loadFontFromFile(const char *path);

  void loadResources();

  void initWindow();

  void drawField(std::shared_ptr<Field> field, sf::Vector2i cursorPos, sf::Vector2i selectionPos);

  void drawSpriteAtFieldPosition(sf::Sprite *sprite, sf::Vector2i position);

  void drawStatus(std::shared_ptr<Field> field);

private:
  sf::Sprite *pegSprite;
  sf::Sprite *selectedPegSprite;
  sf::Sprite *emptyCursorSprite;
  sf::Sprite *cursorSprite;
  sf::Sprite *selectedCursorSprite;
  sf::Sprite *blankTileSprite;

  sf::Font *textFont;

  sf::RenderWindow *appWindow;
};
