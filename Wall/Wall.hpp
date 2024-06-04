#pragma once

#include "RigidBody.hpp"
#include "SFML/System/Vector2.hpp"

namespace game {

class Wall : public RigidBody {
public:
  Wall();

  void setScale(const sf::Vector2f &scale);

  void setScale(float x, float y);

  void resetHitbox();
};

} // namespace game
