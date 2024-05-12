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

} // namespace game
