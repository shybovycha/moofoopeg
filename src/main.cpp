#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <cstdlib>
#include <iostream>
#include <string>

// #define FIELD_SIZE 7
#define START_ROW 3
#define START_COL 3

#define NOT_A_FIELD -1
#define EMPTY 0
#define PEG 1

class Field {
public:
  Field() : data(0), size(sf::Vector2u(0, 0)), startPos(sf::Vector2u(-1, -1)) {}

  ~Field() {
    for (int i = 0; i < size.x; ++i) {
      delete[] data[i];
    }

    delete[] data;
  }

  void initialize(int **data, sf::Vector2u size, sf::Vector2u startPos) {
    this->size = size;
    this->startPos = startPos;

    this->data = new int*[size.y];

    for (int i = 0; i < size.y; ++i) {
      this->data[i] = new int[size.x];
    }

    for (int i = 0; i < size.y; ++i) {
      for (int t = 0; t < size.x; ++t) {
        if ((i < 2 && t < 2) || (i < 2 && t > 4) || (i > 4 && t > 4) || (i > 4 && t < 2)) {
          this->data[i][t] = NOT_A_FIELD;
        } else {
          this->data[i][t] = PEG;
        }
      }
    }

    this->data[startPos.y][startPos.x] = EMPTY;
  }

  bool isMoveValid(int rowFrom, int colFrom, int rowTo, int colTo) {
    int dr = rowTo - rowFrom;
    int dc = colTo - colFrom;

    if (rowFrom >= size.y || rowTo >= size.y || colFrom >= size.x || colTo >= size.x) {
      return 0;
    }

    if (rowFrom < 0 || rowTo < 0 || colFrom < 0 || colTo < 0) {
      return 0;
    }

    return (data[rowFrom][colFrom] == PEG &&
        data[rowTo][colTo] == EMPTY &&
        data[rowFrom + (dr / 2)][colFrom + (dc / 2)] == PEG);  // TODO: rework to check there are holes all the way long
  }

  int countMovesRemaining() {
    int cnt = 0;

    for (int i = 0; i < size.y; ++i) {
      for (int t = 0; t < size.x; ++t) {
        if (isMoveValid(i, t, i + 2, t) ||
          isMoveValid(i, t, i - 2, t) ||
          isMoveValid(i, t, i, t + 2) ||
          isMoveValid(i, t, i, t - 2)
        ) {
          ++cnt;
        }
      }
    }

    return cnt;
  }

  void swapPegs(int rowFrom, int colFrom, int rowTo, int colTo) {
    int dr = rowTo - rowFrom;
    int dc = colTo - colFrom;

    if (!isMoveValid(rowFrom, colFrom, rowTo, colTo)) {
      return;
    }

    if ((dr == 0 && abs(dc) == 2) || (abs(dr) == 2 && dc == 0)) {
      data[rowFrom][colFrom] = EMPTY;
      data[rowFrom + (dr / 2)][colFrom + (dc / 2)] = EMPTY;
      data[rowTo][colTo] = PEG;
    }
  }

  sf::Vector2u getSize() {
    return size;
  }

  bool isValidPos(int row, int col) {
    return row > -1 && col > -1 && row < size.y && col < size.x;
  }

  bool isAvailable(unsigned int row, unsigned int col) {
    return isValidPos(row, col) && data[row][col] != NOT_A_FIELD;
  }

  bool isEmpty(unsigned int row, unsigned int col) {
    return isValidPos(row, col) && data[row][col] == EMPTY;
  }

private:
  int **data;
  sf::Vector2u size;
  sf::Vector2u startPos;
};

class Renderer {
public:
  Renderer() {}

  ~Renderer() {
    delete pegSprite;
    delete selectedPegSprite;
    delete emptyCursorSprite;
    delete cursorSprite;
    delete selectedCursorSprite;
    delete blankTileSprite;

    delete textFont;

    delete appWindow;
  }

  void initialize() {
    loadResources();

    initWindow();
  }

  void draw(Field *field, sf::Vector2i cursorPos, sf::Vector2i selectionPos) {
    // Clear screen
    appWindow->clear(sf::Color::Black);

    // Draw everything
    drawField(field, cursorPos, selectionPos);

    // Draw some text
    drawStatus(field);

    // Finally, display the rendered frame on screen
    appWindow->display();
  }

  sf::Event* pollEvent() {
    sf::Event *event = new sf::Event();

    if (!appWindow->pollEvent(*event)) {
      return NULL;
    }

    return event;
  }

protected:
  sf::Sprite* loadSpriteFromFile(const char *path) {
    sf::Texture *texture = new sf::Texture();

    if (!texture->loadFromFile(path)) {
      delete texture;
      std::string exception = "Can not load image: `" + std::string(path) + "`";
      throw exception.c_str();
    }

    return new sf::Sprite(*texture);
  }

  sf::Font* loadFontFromFile(const char *path) {
    sf::Font *font = new sf::Font();

    if (!font->loadFromFile(path)) {
      delete font;
      std::string exception = "Can not load font: `" + std::string(path) + "`";
      throw exception.c_str();
    }

    return font;
  }

  void loadResources() {
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

  void initWindow() {
    std::cout << "Opening a window..." << std::endl;

    appWindow = new sf::RenderWindow(sf::VideoMode(800, 600), "MooFooPeg", sf::Style::Default, sf::ContextSettings(32));

    appWindow->setVerticalSyncEnabled(true);
    appWindow->setActive(true);

    std::cout << "Window opened" << std::endl;
  }

  void drawField(Field *field, sf::Vector2i cursorPos, sf::Vector2i selectionPos) {
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

  void drawSpriteAtFieldPosition(sf::Sprite *sprite, sf::Vector2i position) {
    sprite->setPosition(sf::Vector2f(position.x * sprite->getTextureRect().width, position.y * sprite->getTextureRect().height));
    appWindow->draw(*sprite);
  }

  void drawStatus(Field *field) {
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

class Game {
public:
  Game() {
    field = new Field();
    renderer = new Renderer();

    cursorPos = sf::Vector2i(START_COL, START_ROW);
    selectionPos = sf::Vector2i(-1, -1);
  }

  ~Game() {
    delete field;
    delete renderer;
  }

  void initialize(int **fieldData, sf::Vector2u fieldSize, sf::Vector2u startPos) {
    field->initialize(fieldData, fieldSize, startPos);
    renderer->initialize();
  }

  void run() {
    while (handleEvents()) {
      draw();
    }
  }

protected:
  bool handleEvents() {
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

  void draw() {
    renderer->draw(field, cursorPos, selectionPos);
  }

private:
  Field *field;
  Renderer *renderer;

  sf::Vector2i cursorPos;
  sf::Vector2i selectionPos;
};

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
