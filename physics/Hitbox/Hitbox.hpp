#pragma once

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/System/Vector2.hpp"

namespace game {

class Hitbox : public sf::RectangleShape {
public:
  sf::Vector2f activeDirection;

  Hitbox(const sf::Vector2f &size, const sf::Vector2f &origin,
         const sf::Vector2f &position,
         const sf::Vector2f &activeDirection_ = {0.f, 0.f});

  sf::Vector2f collisionNormal(const Hitbox &other) const;

  sf::Vector2f getCenterPosition() const;

  bool intersects(const Hitbox &other) const;

  bool isInActiveDirection(const Hitbox &other) const;

  bool isInActiveDirection(const Hitbox &other,
                           const sf::Vector2f &referencePoint) const;

private:
  static const float diagonalNormalSlopeEps;
};

} // namespace game
