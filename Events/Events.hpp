#pragma once

#include "Emitter.hpp"
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

struct Emitters {
  Emitter<Keyboard> keyboard;
  Emitter<RigidBody> rbAdd;
  Emitter<RigidBody> rbRemove;
};

} // namespace game::events
