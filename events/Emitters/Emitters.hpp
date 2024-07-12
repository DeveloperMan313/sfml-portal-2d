#pragma once

#include "Emitter.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "Singleton.hpp"
#include <cstddef>

namespace game {

namespace events {

struct Keyboard {
  sf::Event::EventType type;
  sf::Keyboard::Key key;
};

struct RigidBody {
  size_t rbId;
};

} // namespace events

struct EmittersIns {
  Emitter<events::Keyboard> keyboard;
  Emitter<events::RigidBody> rbAdd;
  Emitter<events::RigidBody> rbRemove;
};

using Emitters = Singleton<EmittersIns>;

} // namespace game