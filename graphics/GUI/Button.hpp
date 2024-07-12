#pragma once

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Text.hpp"
#include <string>

namespace game {

  class Button {
  public:
    Button(sf::Vector2f newPosition, sf::Vector2i newSize, const std::string& pressedTextureName, const std::string& unpressedTextureName, const std::string& text);

    bool check(sf::Vector2i mousePosition);

    void update(sf::Vector2i mousePosition, bool buttonPressed);

    sf::Sprite* getSpritePointer();

    sf::Text* getTextPointer();

    sf::Vector2f position;

    sf::Vector2i size;


  private:
    sf::Texture pressedButton;

    sf::Texture unpressedButton;

    sf::Sprite pressSprite;

    sf::Sprite unpressSprite;

    sf::Font font;

    sf::Text text;

    bool buttonPressed;

  };

}