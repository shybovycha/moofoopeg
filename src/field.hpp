#pragma once

#include <SFML/Graphics.hpp>

enum struct FieldElement {
    NOT_A_FIELD = -1,
    EMPTY = 0,
    PEG = 1
};

class Field {
public:
    Field(std::vector<std::vector<FieldElement>> data, sf::Vector2u startPos);

    ~Field() = default;

    bool isMoveValid(int rowFrom, int colFrom, int rowTo, int colTo);

    int countMovesRemaining();

    void swapPegs(int rowFrom, int colFrom, int rowTo, int colTo);

    sf::Vector2u getSize();

    bool isValidPos(int row, int col);

    bool isAvailable(unsigned int row, unsigned int col);

    bool isEmpty(unsigned int row, unsigned int col);

private:
    std::vector<std::vector<FieldElement>> data;
    sf::Vector2u size;
    sf::Vector2u startPos;

    bool isDirty;
    int lastMoveCount;
};
