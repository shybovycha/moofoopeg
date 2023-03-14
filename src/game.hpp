#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <cstdlib>
#include <iostream>
#include <string>
#include <memory>

#include "field.hpp"
#include "renderer.hpp"

class Game {
public:
    Game(std::vector<std::vector<FieldElement>> fieldData, sf::Vector2u startPos);

    ~Game() = default;

    void run();

protected:
    bool handleEvents();

    void draw();

private:
    std::shared_ptr<Field> field;
    std::shared_ptr<Renderer> renderer;

    sf::Vector2i cursorPos;
    sf::Vector2i selectionPos;
};
