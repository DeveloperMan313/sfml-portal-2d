#pragma once

#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"

namespace game {

class RigidBody;

class Hitbox {
public:
  sf::Vector2f activeDirection;

  Hitbox(const sf::FloatRect &rect_, const sf::Vector2f &origin,
         const sf::Vector2f &activeDirection_ = {0.f, 0.f});

  sf::Vector2f collisionNormal(const Hitbox &other) const;

  void setPosition(const sf::Vector2f &position);

  void setSize(const sf::Vector2f &size);

  void setScale(const sf::Vector2f &scale);

  bool intersects(const Hitbox &other) const;

private:
  sf::RectangleShape rect;
};

} // namespace game
