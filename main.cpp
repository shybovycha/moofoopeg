#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

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
  RenderWindow App(VideoMode(800, 600), "MooFooPeg");
  App.PreserveOpenGLStates(true);

  // Peg
  Image PegImage;

  if (!PegImage.LoadFromFile("peg.jpg")) {
    return EXIT_FAILURE;
  }

  Sprite Peg(PegImage);

  // Selected peg
  Image SelImage;

  if (!SelImage.LoadFromFile("sel_peg.jpg")) {
    return EXIT_FAILURE;
  }

  Sprite SelPeg(SelImage);

  // Empty cursor
  Image EmptyCursorImage;

  if (!EmptyCursorImage.LoadFromFile("cur_hole.jpg")) {
    return EXIT_FAILURE;
  }

  Sprite EmptyCursor(EmptyCursorImage);

  // Cursor over peg
  Image PegCursorImage;

  if (!PegCursorImage.LoadFromFile("cur_peg.jpg")) {
    return EXIT_FAILURE;
  }

  Sprite PegCursor(PegCursorImage);

  // Cursor over selected peg
  Image SelPegCursorImage;

  if (!SelPegCursorImage.LoadFromFile("cur_sel.jpg")) {
    return EXIT_FAILURE;
  }

  Sprite SelPegCursor(SelPegCursorImage);

  // Hole
  Image HoleImage;

  if (!HoleImage.LoadFromFile("hole.jpg")) {
    return EXIT_FAILURE;
  }

  Sprite Hole(HoleImage);

  // Startup
  // Cursor and selected peg
  int curRow = 3;
  int curCol = 3;
  int selRow = -1;
  int selCol = -1;

  fillField();

  // Setup a perspective projection
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(90.f, 1.f, 1.f, 500.f);

  // Start game loop
  while (App.IsOpened()) {
    // Process events
    Event Event;

    while (App.GetEvent(Event)) {
      // Close window : exit
      if (Event.Type == Event::Closed) {
        App.Close();
      }

      // Escape key : exit
      if ((Event.Type == Event::KeyPressed) && (Event.Key.Code == Key::Escape)) {
        App.Close();
        return 0;
      }

      // Manipulation
      if (Event.Type == Event::KeyReleased) {
        if (Event.Key.Code == Key::Right && FIELD[curRow][curCol + 1] > -1)
          curCol++; else
        if (Event.Key.Code == Key::Left && FIELD[curRow][curCol - 1] > -1)
          curCol--; else
        if (Event.Key.Code == Key::Up && FIELD[curRow - 1][curCol] > -1)
          curRow--; else
        if (Event.Key.Code == Key::Down && FIELD[curRow + 1][curCol] > -1)
          curRow++;

        if (Event.Key.Code == Key::Space) {
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

      // Adjust the viewport when the window is resized
      if (Event.Type == Event::Resized) {
        glViewport(0, 0, Event.Size.Width, Event.Size.Height);
      }
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw everything
    for (int i = 0; i < SIZE; i++) {
      for (int t = 0; t < SIZE; t++) {
        if (curRow == i && curCol == t) {
          if (selRow == curRow && selCol == curCol) {
            // cursor here
            SelPegCursor.SetPosition(Vector2f(t * SelPegCursor.GetSize().x, i * SelPegCursor.GetSize().y));
            App.Draw(SelPegCursor);
          } else if (FIELD[i][t] > 0) {
            PegCursor.SetPosition(Vector2f(t * PegCursor.GetSize().x, i * PegCursor.GetSize().y));
            App.Draw(PegCursor);
          } else {
            EmptyCursor.SetPosition(Vector2f(t * EmptyCursor.GetSize().x, i * EmptyCursor.GetSize().y));
            App.Draw(EmptyCursor);
          }

          continue;
        }

        if (selRow == i && selCol == t && selRow != -1 && selCol != -1) {
          // selected peg here
          SelPeg.SetPosition(Vector2f(t * SelPeg.GetSize().x, i * SelPeg.GetSize().y));
          App.Draw(SelPeg);

          continue;
        }

        if (FIELD[i][t] == 0) {
          // hole here
          Hole.SetPosition(Vector2f(t * Hole.GetSize().x, i * Hole.GetSize().y));
          App.Draw(Hole);
        } else if (FIELD[i][t] > 0) {
          // peg here
          Peg.SetPosition(Vector2f(t * Peg.GetSize().x, i * Peg.GetSize().y));
          App.Draw(Peg);
        }
      }
    }

    // Draw some text
    int cnt = countMoves();

    if (cnt > 0) {
      char *s = new char[255];

      sprintf(s, "Moves left:%d", cnt);

      String Text(s);
      Text.SetPosition(250.f, 10.f);
      App.Draw(Text);
    } else {
      String Text("Game over!");
      Text.SetStyle(sf::String::Bold);
      //Text.SetPosition(300.f, 100.f);
      Text.SetPosition((App.GetWidth() / 4) - (Text.GetRect().GetWidth() / 2), (App.GetHeight() / 4) - (Text.GetRect().GetHeight() / 2));
      Text.SetColor(Color(255, 0, 0));
      Text.Rotate(30);
      App.Draw(Text);
    }

    // Finally, display the rendered frame on screen
    App.Display();
  }

  return 0;
}
