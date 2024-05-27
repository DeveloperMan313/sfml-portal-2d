#pragma once

#include "../Emitter/Emitter.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"

namespace game::events {

struct keyboard {
  sf::Event::EventType type;
  sf::Keyboard::Key key;
};

struct emitters {
  Emitter<keyboard> keyboard;
};

} // namespace game::events
