#include "Button.hpp"

namespace game {

  Button::Button(sf::Vector2f newPosition, sf::Vector2i newSize, const std::string& pressedTextureName, const std::string& unpressedTextureName, const std::string& newText){
    position = newPosition;
    size = newSize;

    buttonPressed = false;

    pressSprite.setTexture(*Textures::textures->getTexturePointer(pressedTextureName));
    unpressSprite.setTexture(*Textures::textures->getTexturePointer(unpressedTextureName));

    pressSprite.setScale(3, 3);
    unpressSprite.setScale(3, 3);

    font.loadFromFile("data/fonts/UbuntuSansMono.ttf");
    text.setFont(font);
    text.setString(newText);
    text.setFillColor(sf::Color(0.f, 107, 139));
    text.setOutlineColor(sf::Color(47, 79, 79));

    pressSprite.setPosition(position);
    unpressSprite.setPosition(position);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, 0);
    text.setPosition(sf::Vector2f(newPosition.x + size.x / 2.f, position.y + size.y / 4 - 5));

  }

  bool Button::check(sf::Vector2i mousePosition){
    if (mousePosition.x > position.x && mousePosition.x < position.x + size.x)
      if (mousePosition.y > position.y && mousePosition.y < position.y + size.y)
        return true;
    return false;

  }

  void Button::update(sf::Vector2i mousePosition, bool bp){
    if (check(mousePosition) && bp)
      buttonPressed = true;
    else if (check(mousePosition) && !bp) {
      buttonPressed = false;
    }
    else
      buttonPressed = false;

  }

  sf::Sprite* Button::getSpritePointer()
  {
    if (buttonPressed)
      return &pressSprite;
    else
      return &unpressSprite;

  }

  sf::Text* Button::getTextPointer()
  {
    return &text;

  }

}