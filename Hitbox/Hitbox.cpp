#include "Hitbox.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

namespace game {

Hitbox::Hitbox(const sf::FloatRect &rect_, const sf::Vector2f &origin,
               const sf::Vector2f &activeDirection_)
    : activeDirection(activeDirection_) {
  this->rect.setSize(rect_.getSize());
  this->rect.setOrigin(origin);
}

sf::Vector2f Hitbox::collisionNormal(const Hitbox &other) const {
  if (!this->intersects(other)) {
    return {0.f, 0.f};
  }
  const sf::FloatRect thisGlobalBounds = this->rect.getGlobalBounds(),
                      otherGlobalBounds = other.rect.getGlobalBounds();
  const sf::Vector2f thisSize = thisGlobalBounds.getSize(),
                     otherSize = otherGlobalBounds.getSize();
  const sf::Vector2f thisCenterPos =
      sf::Vector2f(thisGlobalBounds.left, thisGlobalBounds.top) +
      sf::Vector2f(thisGlobalBounds.width, thisGlobalBounds.height) * 0.5f;
  const sf::Vector2f otherCenterPos =
      sf::Vector2f(otherGlobalBounds.left, otherGlobalBounds.top) +
      sf::Vector2f(otherGlobalBounds.width, otherGlobalBounds.height) * 0.5f;
  if (abs(thisCenterPos.x - otherCenterPos.x) / (thisSize.x + otherSize.x) <
      abs(thisCenterPos.y - otherCenterPos.y) / (thisSize.y + otherSize.y)) {
    return {0.f, 1.f};
  }
  return {1.f, 0.f};
}

void Hitbox::setPosition(const sf::Vector2f &position) {
  this->rect.setPosition(position);
}

void Hitbox::setSize(const sf::Vector2f &size) { this->rect.setSize(size); }

void Hitbox::setScale(const sf::Vector2f &scale) { this->rect.setScale(scale); }

bool Hitbox::intersects(const Hitbox &other) const {
  return this->rect.getGlobalBounds().intersects(other.rect.getGlobalBounds());
}

} // namespace game
