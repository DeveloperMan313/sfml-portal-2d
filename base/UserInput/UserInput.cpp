#include "UserInput.hpp"
#include "Emitters.hpp"
#include <SFML/System/Vector2.hpp>

namespace game {

UserInputIns::UserInputIns() : keyStatus(sf::Keyboard::KeyCount, false) {}

bool UserInputIns::checkProcessEvent(const sf::Event &event) {
  switch (event.type) {
  case sf::Event::KeyPressed:
  case sf::Event::KeyReleased:
    if (this->changesKeyStatus(event)) {
      Emitters::get().keyboard.emit(
          {.type = event.type, .key = event.key.code});
    }
    return true;
  case sf::Event::MouseButtonPressed:
  case sf::Event::MouseButtonReleased:
    Emitters::get().mouse.emit(
        {.type = event.type,
         .button = event.mouseButton.button,
         .mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y)});
    return true;
  default:
    break;
  }
  return false;
}

bool UserInputIns::changesKeyStatus(const sf::Event &event) {
  const bool status = event.type == sf::Event::KeyPressed;
  if (this->keyStatus[event.key.code] != status) {
    this->keyStatus[event.key.code] = status;
    return true;
  }
  return false;
}

void UserInputIns::operator delete(void *ptr) noexcept {}

}; // namespace game
