#include <vector>
#include <memory>

#include "game.hpp"

int main() {
    std::vector<std::vector<FieldElement>> field = {
        { FieldElement::NOT_A_FIELD, FieldElement::NOT_A_FIELD, FieldElement::PEG, FieldElement::PEG,   FieldElement::PEG, FieldElement::NOT_A_FIELD, FieldElement::NOT_A_FIELD },
        { FieldElement::NOT_A_FIELD, FieldElement::NOT_A_FIELD, FieldElement::PEG, FieldElement::PEG,   FieldElement::PEG, FieldElement::NOT_A_FIELD, FieldElement::NOT_A_FIELD },
        { FieldElement::PEG,         FieldElement::PEG,         FieldElement::PEG, FieldElement::PEG,   FieldElement::PEG, FieldElement::PEG,         FieldElement::PEG },
        { FieldElement::PEG,         FieldElement::PEG,         FieldElement::PEG, FieldElement::EMPTY, FieldElement::PEG, FieldElement::PEG,         FieldElement::PEG },
        { FieldElement::PEG,         FieldElement::PEG,         FieldElement::PEG, FieldElement::PEG,   FieldElement::PEG, FieldElement::PEG,         FieldElement::PEG },
        { FieldElement::NOT_A_FIELD, FieldElement::NOT_A_FIELD, FieldElement::PEG, FieldElement::PEG,   FieldElement::PEG, FieldElement::NOT_A_FIELD, FieldElement::NOT_A_FIELD },
        { FieldElement::NOT_A_FIELD, FieldElement::NOT_A_FIELD, FieldElement::PEG, FieldElement::PEG,   FieldElement::PEG, FieldElement::NOT_A_FIELD, FieldElement::NOT_A_FIELD }
    };

    auto game = std::make_unique<Game>(std::move(field), sf::Vector2u(3, 3));
    game->run();

    return 0;
}
