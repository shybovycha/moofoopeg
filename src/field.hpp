#pragma once

#include <SFML/Graphics.hpp>

enum class FieldElement { NOT_A_FIELD = -1, EMPTY = 0, PEG = 1 };

class Field {
public:
  Field();

  ~Field();

  void initialize(FieldElement** data, sf::Vector2u size, sf::Vector2u startPos);

  bool isMoveValid(int rowFrom, int colFrom, int rowTo, int colTo);

  int countMovesRemaining();

  void swapPegs(int rowFrom, int colFrom, int rowTo, int colTo);

  sf::Vector2u getSize();

  bool isValidPos(int row, int col);

  bool isAvailable(unsigned int row, unsigned int col);

  bool isEmpty(unsigned int row, unsigned int col);

private:
  FieldElement** data;
  sf::Vector2u size;
  sf::Vector2u startPos;
};
