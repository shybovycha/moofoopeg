#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <cstdlib>
#include <iostream>
#include <string>
#include <format>
#include <memory>

#include "field.hpp"

class Renderer {
public:
    Renderer();

    ~Renderer() = default;

    void initialize();

    void draw(std::shared_ptr<Field> field, sf::Vector2i cursorPos, sf::Vector2i selectionPos);

    sf::Event* pollEvent();

protected:
    std::shared_ptr<sf::Texture> loadTextureFromFile(std::string path);

    std::shared_ptr<sf::Font> loadFontFromFile(std::string path);

    void loadResources();

    void initWindow();

    void drawField(std::shared_ptr<Field> field, sf::Vector2i cursorPos, sf::Vector2i selectionPos);

    void drawSpriteAtFieldPosition(std::shared_ptr<sf::Texture> texture, sf::Vector2i position, sf::Vector2f offset);

    void drawStatus(std::shared_ptr<Field> field);

private:
    std::shared_ptr<sf::Texture> pegTexture;
    std::shared_ptr<sf::Texture> selectedPegTexture;
    std::shared_ptr<sf::Texture> emptyCursorTexture;
    std::shared_ptr<sf::Texture> cursorTexture;
    std::shared_ptr<sf::Texture> selectedCursorTexture;
    std::shared_ptr<sf::Texture> blankTileTexture;

    std::shared_ptr<sf::Font> textFont;

    std::unique_ptr<sf::RenderWindow> appWindow;
};
