#include "Hitbox.hpp"
#include "../Math/Math.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

namespace game {

const float Hitbox::diagonalNormalSlopeEps = 0.01f;

Hitbox::Hitbox(const sf::Vector2f &size, const sf::Vector2f &origin,
               const sf::Vector2f &position,
               const sf::Vector2f &activeDirection_)
    : activeDirection(activeDirection_) {
  this->rect.setSize(size);
  this->rect.setOrigin(origin);
  this->rect.setPosition(position);
}

sf::Vector2f Hitbox::collisionNormal(const Hitbox &other) const {
  if (!this->intersects(other)) {
    return {0.f, 0.f};
  }
  const sf::FloatRect thisGlobalBounds = this->rect.getGlobalBounds(),
                      otherGlobalBounds = other.rect.getGlobalBounds();
  const sf::Vector2f thisSize = thisGlobalBounds.getSize(),
                     otherSize = otherGlobalBounds.getSize();
  const sf::Vector2f thisCenterPos = this->getCenterPosition(),
                     otherCenterPos = other.getCenterPosition();
  const float ratioX = std::abs(thisCenterPos.x - otherCenterPos.x) /
                       (thisSize.x + otherSize.x),
              ratioY = std::abs(thisCenterPos.y - otherCenterPos.y) /
                       (thisSize.y + otherSize.y);
  if (std::abs(ratioX / ratioY - 1.f) < Hitbox::diagonalNormalSlopeEps) {
    return Math::normalize({1.f, 1.f});
  }
  if (ratioX < ratioY) {
    return {0.f, 1.f};
  }
  return {1.f, 0.f};
}

sf::Vector2f Hitbox::getCenterPosition() const {
  const sf::FloatRect globalBounds = this->rect.getGlobalBounds();
  return sf::Vector2f(globalBounds.left, globalBounds.top) +
         sf::Vector2f(globalBounds.width, globalBounds.height) * 0.5f;
}

void Hitbox::setPosition(const sf::Vector2f &position) {
  this->rect.setPosition(position);
}

void Hitbox::setSize(const sf::Vector2f &size) { this->rect.setSize(size); }

void Hitbox::setScale(const sf::Vector2f &scale) { this->rect.setScale(scale); }

bool Hitbox::intersects(const Hitbox &other) const {
  return this->rect.getGlobalBounds().intersects(other.rect.getGlobalBounds());
}

sf::FloatRect Hitbox::getGlobalBounds() const {
  return this->rect.getGlobalBounds();
}

} // namespace game
