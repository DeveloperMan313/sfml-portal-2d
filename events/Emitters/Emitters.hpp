#pragma once

#include "Emitter.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "Singleton.hpp"
#include <SFML/System/Vector2.hpp>
#include <cstddef>

namespace game {

namespace events {

struct Keyboard {
  sf::Event::EventType type;
  sf::Keyboard::Key key;
};

struct Mouse {
  sf::Event::EventType type;
  sf::Mouse::Button button;
  sf::Vector2f mousePos;
};

struct RigidBody {
  size_t rbId;
};

} // namespace events

struct EmittersIns {
  Emitter<events::Keyboard> keyboard;
  Emitter<events::Mouse> mouse;
  Emitter<events::RigidBody> rbAdd;
  Emitter<events::RigidBody> rbRemove;
};

using Emitters = Singleton<EmittersIns>;

} // namespace game
