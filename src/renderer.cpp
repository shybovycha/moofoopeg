#include "renderer.hpp"

Renderer::Renderer() {}

Renderer::~Renderer() {
  delete pegSprite;
  delete selectedPegSprite;
  delete emptyCursorSprite;
  delete cursorSprite;
  delete selectedCursorSprite;
  delete blankTileSprite;

  delete textFont;

  delete appWindow;
}

void Renderer::initialize() {
  loadResources();

  initWindow();
}

void Renderer::draw(std::shared_ptr<Field> field, sf::Vector2i cursorPos, sf::Vector2i selectionPos) {
  // Clear screen
  appWindow->clear(sf::Color::Black);

  // Draw everything
  drawField(field, cursorPos, selectionPos);

  // Draw some text
  drawStatus(field);

  // Finally, display the rendered frame on screen
  appWindow->display();
}

sf::Event* Renderer::pollEvent() {
  sf::Event *event = new sf::Event();

  if (!appWindow->pollEvent(*event)) {
    return nullptr;
  }

  return event;
}

sf::Sprite* Renderer::loadSpriteFromFile(const char *path) {
  sf::Texture *texture = new sf::Texture();

  if (!texture->loadFromFile(path)) {
    delete texture;
    std::string exception = "Can not load image: `" + std::string(path) + "`";
    throw exception.c_str();
  }

  return new sf::Sprite(*texture);
}

sf::Font* Renderer::loadFontFromFile(const char *path) {
  sf::Font *font = new sf::Font();

  if (!font->loadFromFile(path)) {
    delete font;
    std::string exception = "Can not load font: `" + std::string(path) + "`";
    throw exception.c_str();
  }

  return font;
}

void Renderer::loadResources() {
  std::cout << "Loading sprites..." << std::endl;

  pegSprite = loadSpriteFromFile("assets/peg.jpg");
  selectedPegSprite = loadSpriteFromFile("assets/sel_peg.jpg");
  emptyCursorSprite = loadSpriteFromFile("assets/cur_hole.jpg");
  cursorSprite = loadSpriteFromFile("assets/cur_peg.jpg");
  selectedCursorSprite = loadSpriteFromFile("assets/cur_sel.jpg");
  blankTileSprite = loadSpriteFromFile("assets/hole.jpg");

  std::cout << "Loading fonts..." << std::endl;

  textFont = loadFontFromFile("assets/arial.ttf");

  std::cout << "Resources loaded" << std::endl;
}

void Renderer::initWindow() {
  std::cout << "Opening a window..." << std::endl;

  appWindow = new sf::RenderWindow(sf::VideoMode(800, 600), "MooFooPeg", sf::Style::Default, sf::ContextSettings(32));

  appWindow->setVerticalSyncEnabled(true);
  appWindow->setActive(true);

  std::cout << "Window opened" << std::endl;
}

void Renderer::drawField(std::shared_ptr<Field> field, sf::Vector2i cursorPos, sf::Vector2i selectionPos) {
  for (int row = 0; row < field->getSize().x; row++) {
    for (int col = 0; col < field->getSize().y; col++) {
      if (cursorPos.x == col && cursorPos.y == row) {
        if (selectionPos.x == cursorPos.x && selectionPos.y == cursorPos.y) {
          // cursor here
          drawSpriteAtFieldPosition(selectedCursorSprite, sf::Vector2i(col, row));
        } else if (!field->isEmpty(row, col)) {
          drawSpriteAtFieldPosition(cursorSprite, sf::Vector2i(col, row));
        } else {
          drawSpriteAtFieldPosition(emptyCursorSprite, sf::Vector2i(col, row));
        }

        continue;
      }

      if (selectionPos.x == col && selectionPos.y == row && selectionPos.x != -1 && selectionPos.y != -1) {
        // selected peg here
        drawSpriteAtFieldPosition(selectedPegSprite, sf::Vector2i(col, row));

        continue;
      }

      if (field->isEmpty(row, col)) {
        // hole here
        drawSpriteAtFieldPosition(blankTileSprite, sf::Vector2i(col, row));
      } else if (field->isAvailable(row, col) && !field->isEmpty(row, col)) {
        // peg here
        drawSpriteAtFieldPosition(pegSprite, sf::Vector2i(col, row));
      }
    }
  }
}

void Renderer::drawSpriteAtFieldPosition(sf::Sprite *sprite, sf::Vector2i position) {
  sprite->setPosition(sf::Vector2f(position.x * sprite->getTextureRect().width, position.y * sprite->getTextureRect().height));
  appWindow->draw(*sprite);
}

void Renderer::drawStatus(std::shared_ptr<Field> field) {
  int cnt = field->countMovesRemaining();
  sf::Text *text = new sf::Text();

  if (cnt > 0) {
    std::string s = "Moves left:" + std::to_string(cnt);

    text->setString(s.c_str());
    text->setPosition(250.f, 10.f);
    appWindow->draw(*text);
  } else {
    text->setString("Game over!");
    text->setStyle(sf::Text::Bold);
    text->setPosition((appWindow->getSize().x / 4) - (text->getLocalBounds().width / 2), (appWindow->getSize().y / 4) - (text->getLocalBounds().height / 2));
    text->setFillColor(sf::Color(255, 0, 0));
    text->rotate(30);
    appWindow->draw(*text);
  }
}
