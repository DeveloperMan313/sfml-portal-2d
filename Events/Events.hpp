#pragma once

#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <cstddef>

namespace game::events {

struct Keyboard {
  sf::Event::EventType type;
  sf::Keyboard::Key key;
};

struct RigidBody {
  size_t rbId;
};

} // namespace game::events
