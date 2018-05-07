#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>
#include <string>

#define FIELD_SIZE 7
#define START_ROW 3
#define START_COL 3

#define NOT_A_FIELD -1
#define EMPTY 0
#define PEG 1

#define abs(x) (x < 0) ? -x : x

class Field {
public:
  Field() {
    data = new int*[FIELD_SIZE];

    for (int i = 0; i < FIELD_SIZE; ++i) {
      data[i] = new int[FIELD_SIZE];
    }
  }

  ~Field() {
    for (int i = 0; i < FIELD_SIZE; ++i) {
      delete[] data[i];
    }

    delete[] data;
  }

  void initialize() {
    // data = {
    //   { -1, -1, 1, 1, 1, -1, -1 },
    //   { -1, -1, 1, 1, 1, -1, -1 },
    //   {  1,  1, 1, 1, 1,  1,  1 },
    //   {  1,  1, 1, 0, 1,  1,  1 },
    //   {  1,  1, 1, 1, 1,  1,  1 },
    //   { -1, -1, 1, 1, 1, -1, -1 },
    //   { -1, -1, 1, 1, 1, -1, -1 }
    // };

    for (int i = 0; i < FIELD_SIZE; ++i) {
      for (int t = 0; t < FIELD_SIZE; ++t) {
        if ((i < 2 && t < 2) || (i < 2 && t > 4) || (i > 4 && t > 4) || (i > 4 && t < 2)) {
          data[i][t] = NOT_A_FIELD;
        } else {
          data[i][t] = PEG;
        }
      }
    }

    data[START_ROW][START_COL] = EMPTY;
  }

  bool isMoveValid(int rowFrom, int colFrom, int rowTo, int colTo) {
    int dr = rowTo - rowFrom;
    int dc = colTo - colFrom;

    if (rowFrom >= FIELD_SIZE || rowTo >= FIELD_SIZE || colFrom >= FIELD_SIZE || colTo >= FIELD_SIZE) {
      return 0;
    }

    if (rowFrom < 0 || rowTo < 0 || colFrom < 0 || colTo < 0) {
      return 0;
    }

    return (data[rowFrom][colFrom] == PEG &&
        data[rowTo][colTo] == EMPTY &&
        data[rowFrom + (dr / 2)][colFrom + (dc / 2)] == PEG // TODO: rework to check there are holes all the way long
    );
  }

  int countMovesRemaining() {
    int cnt = 0;

    for (int i = 0; i < FIELD_SIZE; ++i) {
      for (int t = 0; t < FIELD_SIZE; ++t) {
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

  void swap(int rowFrom, int colFrom, int rowTo, int colTo) {
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

  sf::Vector2i getSize() {
    return sf::Vector2i(FIELD_SIZE, FIELD_SIZE);
  }

  // // checks whether there is only one peg left on the board
  // bool checkWin() {
  //   int cnt = 0;

  //   for (int i = 0; i < FIELD_SIZE; ++i) {
  //     for (int t = 0; t < FIELD_SIZE; ++t) {
  //       if (data[i][t] == PEG) {
  //         ++cnt;
  //       }
  //     }
  //   }

  //   return (cnt == 1);
  // }

  bool isValidPos(int row, int col) {
    return row > -1 && col > -1 && row < FIELD_SIZE && col < FIELD_SIZE;
  }

  bool isAvailable(unsigned int row, unsigned int col) {
    return isValidPos(row, col) && data[row][col] != NOT_A_FIELD;
  }

  bool isEmpty(unsigned int row, unsigned int col) {
    return isValidPos(row, col) && data[row][col] == EMPTY;
  }

private:
  int **data;
};

class Renderer {
public:
  Renderer() {}

  ~Renderer() {
    delete Peg;
    delete SelPeg;
    delete EmptyCursor;
    delete PegCursor;
    delete SelPegCursor;
    delete Hole;

    delete font;

    delete App;
  }

  void initialize() {
    loadResources();

    initWindow();
  }

  void draw(Field *field, sf::Vector2i cursorPos, sf::Vector2i selectionPos) {
    // Clear screen
    App->clear(sf::Color::Black);

    // Draw everything
    drawField(field, cursorPos, selectionPos);

    // Draw some text
    drawStatus(field);

    // Finally, display the rendered frame on screen
    App->display();
  }

  sf::Event* pollEvent() {
    sf::Event *evt = new sf::Event();

    if (!App->pollEvent(*evt)) {
      return NULL;
    }

    return evt;
  }

protected:
  sf::Sprite* loadSpriteFromFile(const char *path) {
    sf::Texture *PegImage = new sf::Texture();

    if (!PegImage->loadFromFile(path)) {
      std::string exception = "Can not load image: `" + std::string(path) + "`";
      throw exception.c_str();
    }

    return new sf::Sprite(*PegImage);
  }

  sf::Font* loadFontFromFile(const char *path) {
    sf::Font *Font = new sf::Font();

    if (!Font->loadFromFile(path)) {
      std::string exception = "Can not load font: `" + std::string(path) + "`";

      throw exception.c_str();
    }

    return Font;
  }

  void loadResources() {
    std::cout << "Loading sprites..." << std::endl;

    Peg = loadSpriteFromFile("peg.jpg");
    SelPeg = loadSpriteFromFile("sel_peg.jpg");
    EmptyCursor = loadSpriteFromFile("cur_hole.jpg");
    PegCursor = loadSpriteFromFile("cur_peg.jpg");
    SelPegCursor = loadSpriteFromFile("cur_sel.jpg");
    Hole = loadSpriteFromFile("hole.jpg");

    std::cout << "Loading fonts..." << std::endl;

    font = loadFontFromFile("arial.ttf");

    std::cout << "Resources loaded" << std::endl;
  }

  void initWindow() {
    std::cout << "Opening a window..." << std::endl;

    App = new sf::RenderWindow(sf::VideoMode(800, 600), "MooFooPeg", sf::Style::Default, sf::ContextSettings(32));

    App->setVerticalSyncEnabled(true);
    App->setActive(true);

    std::cout << "Window opened" << std::endl;
  }

  void drawField(Field *field, sf::Vector2i cursorPos, sf::Vector2i selectionPos) {
    for (int i = 0; i < field->getSize().x; i++) {
      for (int t = 0; t < field->getSize().y; t++) {
        if (cursorPos.x == i && cursorPos.y == t) {
          if (selectionPos.x == cursorPos.x && selectionPos.y == cursorPos.y) {
            // cursor here
            drawSpriteAtFieldPosition(SelPegCursor, sf::Vector2i(t, i));
          } else if (!field->isEmpty(i, t)) {
            drawSpriteAtFieldPosition(PegCursor, sf::Vector2i(t, i));
          } else {
            drawSpriteAtFieldPosition(EmptyCursor, sf::Vector2i(t, i));
          }

          continue;
        }

        if (selectionPos.x == i && selectionPos.y == t && selectionPos.x != -1 && selectionPos.y != -1) {
          // selected peg here
          drawSpriteAtFieldPosition(SelPeg, sf::Vector2i(t, i));

          continue;
        }

        if (field->isEmpty(i, t)) {
          // hole here
          drawSpriteAtFieldPosition(Hole, sf::Vector2i(t, i));
        } else if (field->isAvailable(i, t) && !field->isEmpty(i, t)) {
          // peg here
          drawSpriteAtFieldPosition(Peg, sf::Vector2i(t, i));
        }
      }
    }
  }

  void drawSpriteAtFieldPosition(sf::Sprite *sprite, sf::Vector2i position) {
    sprite->setPosition(sf::Vector2f(position.x * sprite->getTextureRect().width, position.y * sprite->getTextureRect().height));
    App->draw(*sprite);
  }

  void drawStatus(Field *field) {
    int cnt = field->countMovesRemaining();

    if (cnt > 0) {
      std::string s = "Moves left:" + cnt;

      sf::Text *Text = new sf::Text();

      Text->setString(s.c_str());
      Text->setPosition(250.f, 10.f);
      App->draw(*Text);
    } else {
      sf::Text *Text = new sf::Text();

      Text->setString("Game over!");
      Text->setStyle(sf::Text::Bold);
      Text->setPosition((App->getSize().x / 4) - (Text->getLocalBounds().width / 2), (App->getSize().y / 4) - (Text->getLocalBounds().height / 2));
      Text->setColor(sf::Color(255, 0, 0));
      Text->rotate(30);
      App->draw(*Text);
    }
  }

private:
  sf::Sprite *Peg;
  sf::Sprite *SelPeg;
  sf::Sprite *EmptyCursor;
  sf::Sprite *PegCursor;
  sf::Sprite *SelPegCursor;
  sf::Sprite *Hole;

  sf::Font *font;

  sf::RenderWindow *App;
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

  void initialize() {
    field->initialize();
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
    sf::Event *evt;

    while ((evt = renderer->pollEvent())) {
      // Close window : exit
      if (evt->type == sf::Event::Closed) {
        return false;
      }

      // Escape key : exit
      if ((evt->type == sf::Event::KeyPressed) && (evt->key.code == sf::Keyboard::Escape)) {
        return false;
      }

      // Manipulation
      if (evt->type == sf::Event::KeyReleased) {
        if (evt->key.code == sf::Keyboard::Right && field->isAvailable(cursorPos.x, cursorPos.y + 1)) {
          cursorPos.y += 1;
        } else if (evt->key.code == sf::Keyboard::Left && field->isAvailable(cursorPos.x, cursorPos.y - 1)) {
          cursorPos.y -= 1;
        } else if (evt->key.code == sf::Keyboard::Up && field->isAvailable(cursorPos.x - 1, cursorPos.y)) {
          cursorPos.x -= 1;
        } else if (evt->key.code == sf::Keyboard::Down && field->isAvailable(cursorPos.x + 1, cursorPos.y)) {
          cursorPos.x += 1;
        }

        if (evt->key.code == sf::Keyboard::Space || evt->key.code == sf::Keyboard::Return) {
          if (selectionPos.y != -1 && selectionPos.x != -1) {
            if (selectionPos.y != cursorPos.y || selectionPos.x != cursorPos.x) {
              field->swap(selectionPos.x, selectionPos.y, cursorPos.x, cursorPos.y);
              selectionPos.y = selectionPos.x = -1;
            } else if (selectionPos.y == cursorPos.y && selectionPos.x == cursorPos.x) {
              selectionPos.y = selectionPos.x = -1;
            }
          } else if (!field->isEmpty(cursorPos.x, cursorPos.y)) {
            selectionPos.y = cursorPos.y;
            selectionPos.x = cursorPos.x;
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

  game->initialize();
  game->run();

  return 0;
}
