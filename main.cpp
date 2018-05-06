#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <stdio.h>

using namespace sf;
using namespace std;

#define SIZE 7

int FIELD[SIZE][SIZE] = {{0}};

#define iabs(x) (x < 0) ? -x : x

void fillField() {
  for (int i = 0; i < SIZE; ++i) {
    for (int t = 0; t < SIZE; ++t) {
      if ((i < 2 && t < 2) ||
        (i > 4 && t > 4) ||
        (i > 4 && t < 2) ||
        (i < 2 && t > 4)) {

        FIELD[i][t] = -1;
      } else {
        FIELD[i][t] = 1;
      }
    }
  }

  FIELD[3][3] = 0;
}

int checkMove(int row0, int col0, int row1, int col1) {
  int dr = row1 - row0;
  int dc = col1 - col0;

  if (row0 >= SIZE || row1 >= SIZE || col0 >= SIZE || col1 >= SIZE) {
    return 0;
  }

  if (row0 < 0 || row1 < 0 || col0 < 0 || col1 < 0) {
    return 0;
  }

  return (FIELD[row0][col0] == 1 &&
      FIELD[row1][col1] == 0 &&
      FIELD[row0 + dr / 2][col0 + dc / 2] == 1
  );
}

int countMoves() {
  int cnt = 0;

  for (int i = 0; i < SIZE; i++) {
    for (int t = 0; t < SIZE; t++) {
      if (checkMove(i, t, i + 2, t) ||
        checkMove(i, t, i - 2, t) ||
        checkMove(i, t, i, t + 2) ||
        checkMove(i, t, i, t - 2)
      ) {
        cnt++;
      }
    }
  }

  return cnt;
}

void swap(int row0, int col0, int row1, int col1) {
  int dr = row1 - row0;
  int dc = col1 - col0;

  if (!checkMove(row0, col0, row1, col1)) {
    return;
  }

  if ((dr == 0 && iabs(dc) == 2) || (iabs(dr) == 2 && dc == 0)) {
    FIELD[row0][col0] = 0;
    FIELD[row0 + dr / 2][col0 + dc / 2] = 0;
    FIELD[row1][col1] = 1;
  }
}

int checkWin() {
  int cnt = 0;

  for (int i = 0; i < SIZE; i++) {
    for (int t = 0; t < SIZE; t++) {
      if (FIELD[i][t] > 0) {
        cnt++;
      }
    }
  }

  return (cnt == 1);
}

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
  int curRow = 3;
  int curCol = 3;
  int selRow = -1;
  int selCol = -1;

  fillField();

  // Setup a perspective projection
  // glMatrixMode(GL_PROJECTION);
  // glLoadIdentity();
  // gluPerspective(90.f, 1.f, 1.f, 500.f);

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
        if (Event.key.code == sf::Keyboard::Right && FIELD[curRow][curCol + 1] > -1)
          curCol++; else
        if (Event.key.code == sf::Keyboard::Left && FIELD[curRow][curCol - 1] > -1)
          curCol--; else
        if (Event.key.code == sf::Keyboard::Up && FIELD[curRow - 1][curCol] > -1)
          curRow--; else
        if (Event.key.code == sf::Keyboard::Down && FIELD[curRow + 1][curCol] > -1)
          curRow++;

        if (Event.key.code == sf::Keyboard::Space) {
          if (selRow != -1 && selCol != -1) {
            if (selRow != curRow || selCol != curCol) {
              swap(selRow, selCol, curRow, curCol);
              selRow = selCol = -1;
            } else if (selRow == curRow && selCol == curCol) {
              selRow = selCol = -1;
            }
          } else if (FIELD[curRow][curCol] > 0) {
            selRow = curRow;
            selCol = curCol;
          }
        }
      }

      // // Adjust the viewport when the window is resized
      // if (Event.type == sf::Event::Resized) {
      //   glViewport(0, 0, Event.Size.Width, Event.Size.Height);
      // }
    }

    if (curRow < 0) {
      curRow = 0;
    }

    if (curCol < 0) {
      curCol = 0;
    }

    if (curRow > SIZE - 1) {
      curRow = SIZE - 1;
    }

    if (curCol > SIZE - 1) {
      curCol = SIZE - 1;
    }

    // ============ MAIN LOGIC GOES HERE =============
    // Clear screen
    App.clear(sf::Color::Black);

    // Draw everything
    for (int i = 0; i < SIZE; i++) {
      for (int t = 0; t < SIZE; t++) {
        if (curRow == i && curCol == t) {
          if (selRow == curRow && selCol == curCol) {
            // cursor here
            SelPegCursor.setPosition(sf::Vector2f(t * SelPegCursor.getTextureRect().width, i * SelPegCursor.getTextureRect().height));
            App.draw(SelPegCursor);
          } else if (FIELD[i][t] > 0) {
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

        if (FIELD[i][t] == 0) {
          // hole here
          Hole.setPosition(sf::Vector2f(t * Hole.getTextureRect().width, i * Hole.getTextureRect().height));
          App.draw(Hole);
        } else if (FIELD[i][t] > 0) {
          // peg here
          Peg.setPosition(sf::Vector2f(t * Peg.getTextureRect().width, i * Peg.getTextureRect().height));
          App.draw(Peg);
        }
      }
    }

    // Draw some text
    int cnt = countMoves();

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
  }

  return 0;
}
