#include "game.hpp"

Game::Game() {
  field = new Field();
  renderer = new Renderer();

  cursorPos = sf::Vector2i(START_COL, START_ROW);
  selectionPos = sf::Vector2i(-1, -1);
}

Game::~Game() {
  delete field;
  delete renderer;
}

void Game::initialize(int **fieldData, sf::Vector2u fieldSize, sf::Vector2u startPos) {
  field->initialize(fieldData, fieldSize, startPos);
  renderer->initialize();
}

void Game::run() {
  while (handleEvents()) {
    draw();
  }
}

bool Game::handleEvents() {
  // Process events
  sf::Event *event;

  while ((event = renderer->pollEvent())) {
    // Close window : exit
    if (event->type == sf::Event::Closed) {
      return false;
    }

    // Escape key : exit
    if ((event->type == sf::Event::KeyPressed) && (event->key.code == sf::Keyboard::Escape)) {
      return false;
    }

    // Manipulation
    if (event->type == sf::Event::KeyReleased) {
      if (event->key.code == sf::Keyboard::Right && field->isAvailable(cursorPos.y, cursorPos.x + 1)) {
        cursorPos.x += 1;
      } else if (event->key.code == sf::Keyboard::Left && field->isAvailable(cursorPos.y, cursorPos.x - 1)) {
        cursorPos.x -= 1;
      } else if (event->key.code == sf::Keyboard::Up && field->isAvailable(cursorPos.y - 1, cursorPos.x)) {
        cursorPos.y -= 1;
      } else if (event->key.code == sf::Keyboard::Down && field->isAvailable(cursorPos.y + 1, cursorPos.x)) {
        cursorPos.y += 1;
      }

      if (event->key.code == sf::Keyboard::Space || event->key.code == sf::Keyboard::Return) {
        if (selectionPos.y != -1 && selectionPos.x != -1) {
          if (selectionPos.y != cursorPos.y || selectionPos.x != cursorPos.x) {
            field->swapPegs(selectionPos.y, selectionPos.x, cursorPos.y, cursorPos.x);
            selectionPos.y = selectionPos.x = -1;
          } else if (selectionPos.y == cursorPos.y && selectionPos.x == cursorPos.x) {
            selectionPos.y = selectionPos.x = -1;
          }
        } else if (!field->isEmpty(cursorPos.y, cursorPos.x)) {
          selectionPos = cursorPos;
        }
      }
    }
  }

  return true;
}

void Game::draw() {
  renderer->draw(field, cursorPos, selectionPos);
}
