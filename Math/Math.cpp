#include "Math.hpp"

namespace game {

float Math::magnitude(const sf::Vector2f &vec) {
  return sqrt(vec.x * vec.x + vec.y * vec.y);
};

float Math::dot(const sf::Vector2f &vec1, const sf::Vector2f &vec2) {
  return vec1.x * vec2.x + vec1.y * vec2.y;
}

sf::Vector2f Math::normalize(const sf::Vector2f &vec) {
  return vec / Math::magnitude(vec);
};

float Math::projection(const sf::Vector2f &vec, const sf::Vector2f &axis) {
  return Math::dot(vec, Math::normalize(axis));
};

float Math::fullAngle(const sf::Vector2f &from, const sf::Vector2f &to) {
  return std::atan2(to.x, to.y) - std::atan2(from.x, from.y);
}

sf::Vector2f Math::rotate(const sf::Vector2f &vec, float angle) {
  return {vec.x * std::cos(angle) + vec.y * -std::sin(angle),
          vec.x * std::sin(angle) + vec.y * std::cos(angle)};
}

} // namespace game
