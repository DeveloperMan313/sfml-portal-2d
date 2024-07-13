#include "UserInput.hpp"
#include "Emitters.hpp"

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
