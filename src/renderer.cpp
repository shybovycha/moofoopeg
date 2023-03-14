#include "renderer.hpp"

Renderer::Renderer() {}

void Renderer::initialize() {
    loadResources();

    initWindow();
}

void Renderer::draw(std::shared_ptr<Field> field, sf::Vector2i cursorPos, sf::Vector2i selectionPos) {
    // Clear screen
    appWindow->clear(sf::Color::Black);

    // Draw everything
    drawField(field, cursorPos, selectionPos);

    // Draw some text
    drawStatus(field);

    // Finally, display the rendered frame on screen
    appWindow->display();
}

sf::Event* Renderer::pollEvent() {
    sf::Event* event = new sf::Event();

    if (!appWindow->pollEvent(*event)) {
        return nullptr;
    }

    return event;
}

std::shared_ptr<sf::Texture> Renderer::loadTextureFromFile(std::string path) {
    auto texture = std::make_shared<sf::Texture>();

    if (!texture->loadFromFile(path.c_str())) {
        auto exception = std::format("Can not load image from file '{}'", path);
        throw exception.c_str();
    }

    return std::move(texture);
}

std::shared_ptr<sf::Font> Renderer::loadFontFromFile(std::string path) {
    auto font = std::make_shared<sf::Font>();

    if (!font->loadFromFile(path)) {
        auto exception = std::format("Can not load font from file '{}'", path);
        throw exception.c_str();
    }

    return std::move(font);
}

void Renderer::loadResources() {
    std::cout << "Loading sprites..." << std::endl;

    pegTexture = loadTextureFromFile("assets/peg.jpg");
    selectedPegTexture = loadTextureFromFile("assets/sel_peg.jpg");
    emptyCursorTexture = loadTextureFromFile("assets/cur_hole.jpg");
    cursorTexture = loadTextureFromFile("assets/cur_peg.jpg");
    selectedCursorTexture = loadTextureFromFile("assets/cur_sel.jpg");
    blankTileTexture = loadTextureFromFile("assets/hole.jpg");

    std::cout << "Loading fonts..." << std::endl;

    textFont = loadFontFromFile("assets/arial.ttf");

    std::cout << "Resources loaded" << std::endl;
}

void Renderer::initWindow() {
    std::cout << "Opening a window..." << std::endl;

    appWindow = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600), "MooFooPeg", sf::Style::Default, sf::ContextSettings());

    appWindow->setVerticalSyncEnabled(true);
    appWindow->setActive(true);

    std::cout << "Window opened" << std::endl;
}

void Renderer::drawField(std::shared_ptr<Field> field, sf::Vector2i cursorPos, sf::Vector2i selectionPos) {
    sf::Vector2f offset{ (appWindow->getSize().x / 2.f) - (field->getSize().x * pegTexture->getSize().x) / 2.f, (appWindow->getSize().y / 2.f) - (field->getSize().y * pegTexture->getSize().y) / 2.f };

    for (int row = 0; row < field->getSize().x; row++) {
        for (int col = 0; col < field->getSize().y; col++) {
            if (cursorPos.x == col && cursorPos.y == row) {
                if (selectionPos.x == cursorPos.x && selectionPos.y == cursorPos.y) {
                    // cursor here
                    drawSpriteAtFieldPosition(selectedCursorTexture, sf::Vector2i(col, row), offset);
                }
                else if (!field->isEmpty(row, col)) {
                    drawSpriteAtFieldPosition(cursorTexture, sf::Vector2i(col, row), offset);
                }
                else {
                    drawSpriteAtFieldPosition(emptyCursorTexture, sf::Vector2i(col, row), offset);
                }

                continue;
            }

            if (selectionPos.x == col && selectionPos.y == row && selectionPos.x != -1 && selectionPos.y != -1) {
                // selected peg here
                drawSpriteAtFieldPosition(selectedPegTexture, sf::Vector2i(col, row), offset);

                continue;
            }

            if (field->isEmpty(row, col)) {
                // hole here
                drawSpriteAtFieldPosition(blankTileTexture, sf::Vector2i(col, row), offset);
            }
            else if (field->isAvailable(row, col) && !field->isEmpty(row, col)) {
                // peg here
                drawSpriteAtFieldPosition(pegTexture, sf::Vector2i(col, row), offset);
            }
        }
    }
}

void Renderer::drawSpriteAtFieldPosition(std::shared_ptr<sf::Texture> texture, sf::Vector2i position, sf::Vector2f offset) {
    auto sprite = std::make_unique<sf::Sprite>(*texture);
    sprite->setPosition(offset + sf::Vector2f(position.x * sprite->getTextureRect().width, position.y * sprite->getTextureRect().height));
    appWindow->draw(*sprite);
}

void Renderer::drawStatus(std::shared_ptr<Field> field) {
    int cnt = field->countMovesRemaining();

    auto text = std::make_unique<sf::Text>();

    text->setFont(*textFont);

    if (cnt > 0) {
        auto s = std::format("Moves left: {}", cnt);

        text->setFillColor(sf::Color(255, 255, 255));
        text->setString(s.c_str());
        text->setPosition((appWindow->getSize().x / 2) - (text->getLocalBounds().width / 2), appWindow->getSize().y - (text->getLocalBounds().height * 2));

        appWindow->draw(*text);
    }
    else {
        text->setString("Game over!");
        text->setPosition((appWindow->getSize().x / 4) - (text->getLocalBounds().width / 2), (appWindow->getSize().y / 4) - (text->getLocalBounds().height / 2));
        text->setStyle(sf::Text::Bold);
        text->setFillColor(sf::Color(255, 0, 0));
        text->rotate(30);
        appWindow->draw(*text);
    }
}
