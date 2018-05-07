#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <stdio.h>

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

int main() {
  sf::RenderWindow App(sf::VideoMode(800, 600), "MooFooPeg", sf::Style::Default, sf::ContextSettings(32));

  App.setVerticalSyncEnabled(true);
  App.setActive(true);

  // Peg
  sf::Texture PegImage;

  if (!PegImage.loadFromFile("peg.jpg")) {
    return EXIT_FAILURE;
  }

  sf::Sprite Peg(PegImage);

  // Selected peg
  sf::Texture SelImage;

  if (!SelImage.loadFromFile("sel_peg.jpg")) {
    return EXIT_FAILURE;
  }

  sf::Sprite SelPeg(SelImage);

  // Empty cursor
  sf::Texture EmptyCursorImage;

  if (!EmptyCursorImage.loadFromFile("cur_hole.jpg")) {
    return EXIT_FAILURE;
  }

  sf::Sprite EmptyCursor(EmptyCursorImage);

  // Cursor over peg
  sf::Texture PegCursorImage;

  if (!PegCursorImage.loadFromFile("cur_peg.jpg")) {
    return EXIT_FAILURE;
  }

  sf::Sprite PegCursor(PegCursorImage);

  // Cursor over selected peg
  sf::Texture SelPegCursorImage;

  if (!SelPegCursorImage.loadFromFile("cur_sel.jpg")) {
    return EXIT_FAILURE;
  }

  sf::Sprite SelPegCursor(SelPegCursorImage);

  // Hole
  sf::Texture HoleImage;

  if (!HoleImage.loadFromFile("hole.jpg")) {
    return EXIT_FAILURE;
  }

  sf::Sprite Hole(HoleImage);

  sf::Font font;

  if (!font.loadFromFile("arial.ttf")) {
    return EXIT_FAILURE;
  }

  // Startup
  // Cursor and selected peg
  int curRow = START_ROW;
  int curCol = START_COL;
  int selRow = -1;
  int selCol = -1;

  Field *field = new Field();

  field->initialize();

  bool stateChanged = true;

  // Start game loop
  while (App.isOpen()) {
    // Process events
    sf::Event Event;

    while (App.pollEvent(Event)) {
      // Close window : exit
      if (Event.type == sf::Event::Closed) {
        App.close();
        return 0;
      }

      // Escape key : exit
      if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape)) {
        App.close();
        return 0;
      }

      // Manipulation
      if (Event.type == sf::Event::KeyReleased) {
        if (Event.key.code == sf::Keyboard::Right && field->isAvailable(curRow, curCol + 1)) {
          curCol++;
          stateChanged = true;
        } else if (Event.key.code == sf::Keyboard::Left && field->isAvailable(curRow, curCol - 1)) {
          curCol--;
          stateChanged = true;
        } else if (Event.key.code == sf::Keyboard::Up && field->isAvailable(curRow - 1, curCol)) {
          curRow--;
          stateChanged = true;
        } else if (Event.key.code == sf::Keyboard::Down && field->isAvailable(curRow + 1, curCol)) {
          curRow++;
          stateChanged = true;
        }

        if (Event.key.code == sf::Keyboard::Space || Event.key.code == sf::Keyboard::Return) {
          if (selRow != -1 && selCol != -1) {
            if (selRow != curRow || selCol != curCol) {
              field->swap(selRow, selCol, curRow, curCol);
              selRow = selCol = -1;
            } else if (selRow == curRow && selCol == curCol) {
              selRow = selCol = -1;
            }

            stateChanged = true;
          } else if (!field->isEmpty(curRow, curCol)) {
            selRow = curRow;
            selCol = curCol;

            stateChanged = true;
          }
        }
      }
    }

    if (!stateChanged) {
      continue;
    }

    if (curRow < 0) {
      curRow = 0;
    }

    if (curCol < 0) {
      curCol = 0;
    }

    if (curRow > FIELD_SIZE - 1) {
      curRow = FIELD_SIZE - 1;
    }

    if (curCol > FIELD_SIZE - 1) {
      curCol = FIELD_SIZE - 1;
    }

    // ============ MAIN LOGIC GOES HERE =============
    // Clear screen
    App.clear(sf::Color::Black);

    // Draw everything
    for (int i = 0; i < FIELD_SIZE; i++) {
      for (int t = 0; t < FIELD_SIZE; t++) {
        if (curRow == i && curCol == t) {
          if (selRow == curRow && selCol == curCol) {
            // cursor here
            SelPegCursor.setPosition(sf::Vector2f(t * SelPegCursor.getTextureRect().width, i * SelPegCursor.getTextureRect().height));
            App.draw(SelPegCursor);
          } else if (!field->isEmpty(i, t)) {
            PegCursor.setPosition(sf::Vector2f(t * PegCursor.getTextureRect().width, i * PegCursor.getTextureRect().height));
            App.draw(PegCursor);
          } else {
            EmptyCursor.setPosition(sf::Vector2f(t * EmptyCursor.getTextureRect().width, i * EmptyCursor.getTextureRect().height));
            App.draw(EmptyCursor);
          }

          continue;
        }

        if (selRow == i && selCol == t && selRow != -1 && selCol != -1) {
          // selected peg here
          SelPeg.setPosition(sf::Vector2f(t * SelPeg.getTextureRect().width, i * SelPeg.getTextureRect().height));
          App.draw(SelPeg);

          continue;
        }

        if (field->isEmpty(i, t)) {
          // hole here
          Hole.setPosition(sf::Vector2f(t * Hole.getTextureRect().width, i * Hole.getTextureRect().height));
          App.draw(Hole);
        } else if (field->isAvailable(i, t) && !field->isEmpty(i, t)) {
          // peg here
          Peg.setPosition(sf::Vector2f(t * Peg.getTextureRect().width, i * Peg.getTextureRect().height));
          App.draw(Peg);
        }
      }
    }

    // Draw some text
    int cnt = field->countMovesRemaining();

    if (cnt > 0) {
      char *s = new char[255];

      sprintf(s, "Moves left:%d", cnt);

      sf::Text Text;

      Text.setString(s);
      Text.setPosition(250.f, 10.f);
      App.draw(Text);
    } else {
      sf::Text Text;

      Text.setString("Game over!");
      Text.setStyle(sf::Text::Bold);
      //Text.setPosition(300.f, 100.f);
      Text.setPosition((App.getSize().x / 4) - (Text.getLocalBounds().width / 2), (App.getSize().y / 4) - (Text.getLocalBounds().height / 2));
      Text.setColor(sf::Color(255, 0, 0));
      Text.rotate(30);
      App.draw(Text);
    }

    // Finally, display the rendered frame on screen
    App.display();
    stateChanged = false;
  }

  delete field;

  return 0;
}
