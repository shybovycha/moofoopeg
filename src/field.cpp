#include "field.hpp"

#define NOT_A_FIELD -1
#define EMPTY 0
#define PEG 1

Field::Field() : data(0), size(sf::Vector2u(0, 0)), startPos(sf::Vector2u(-1, -1)) {}

Field::~Field() {
  for (int i = 0; i < size.x; ++i) {
    delete[] data[i];
  }

  delete[] data;
}

void Field::initialize(FieldElement **data, sf::Vector2u size, sf::Vector2u startPos) {
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

bool Field::isMoveValid(int rowFrom, int colFrom, int rowTo, int colTo) {
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

int Field::countMovesRemaining() {
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

void Field::swapPegs(int rowFrom, int colFrom, int rowTo, int colTo) {
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

sf::Vector2u Field::getSize() {
  return size;
}

bool Field::isValidPos(int row, int col) {
  return row > -1 && col > -1 && row < size.y && col < size.x;
}

bool Field::isAvailable(unsigned int row, unsigned int col) {
  return isValidPos(row, col) && data[row][col] != NOT_A_FIELD;
}

bool Field::isEmpty(unsigned int row, unsigned int col) {
  return isValidPos(row, col) && data[row][col] == EMPTY;
}
