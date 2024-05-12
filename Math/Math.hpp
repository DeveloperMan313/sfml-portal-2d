#pragma once

#include "SFML/System/Vector2.hpp"
#include <cmath>

namespace game {

class Math {
public:
  template <typename T> static int sign(T val) {
    return (T(0) < val) - (val < T(0));
  }

  static float magnitude(const sf::Vector2f &vec);

  static float dot(const sf::Vector2f &vec1, const sf::Vector2f &vec2);

  static sf::Vector2f normalize(const sf::Vector2f &vec);

  static float projection(const sf::Vector2f &vec, const sf::Vector2f &axis);
};

} // namespace game
