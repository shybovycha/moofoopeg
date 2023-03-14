#include "field.hpp"

Field::Field(std::vector<std::vector<FieldElement>> fieldData, sf::Vector2u startPos) :
    data(std::move(fieldData)),
    size(sf::Vector2u(0, 0)),
    startPos(startPos),
    isDirty(false),
    lastMoveCount(0)
{
    size = sf::Vector2u(data.size(), data.size() > 0 ? data[0].size() : 0);
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

    return (data[rowFrom][colFrom] == FieldElement::PEG &&
        data[rowTo][colTo] == FieldElement::EMPTY &&
        data[rowFrom + (dr / 2)][colFrom + (dc / 2)] == FieldElement::PEG);  // TODO: rework to check there are holes all the way long
}

int Field::countMovesRemaining() {
    if (!isDirty) {
        return lastMoveCount;
    }

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

    lastMoveCount = cnt;
    isDirty = false;

    return cnt;
}

void Field::swapPegs(int rowFrom, int colFrom, int rowTo, int colTo) {
    int dr = rowTo - rowFrom;
    int dc = colTo - colFrom;

    if (!isMoveValid(rowFrom, colFrom, rowTo, colTo)) {
        return;
    }

    if ((dr == 0 && abs(dc) == 2) || (abs(dr) == 2 && dc == 0)) {
        data[rowFrom][colFrom] = FieldElement::EMPTY;
        data[rowFrom + (dr / 2)][colFrom + (dc / 2)] = FieldElement::EMPTY;
        data[rowTo][colTo] = FieldElement::PEG;
    }

    isDirty = true;
}

sf::Vector2u Field::getSize() {
    return size;
}

bool Field::isValidPos(int row, int col) {
    return row > -1 && col > -1 && row < size.y&& col < size.x;
}

bool Field::isAvailable(unsigned int row, unsigned int col) {
    return isValidPos(row, col) && data[row][col] != FieldElement::NOT_A_FIELD;
}

bool Field::isEmpty(unsigned int row, unsigned int col) {
    return isValidPos(row, col) && data[row][col] == FieldElement::EMPTY;
}
