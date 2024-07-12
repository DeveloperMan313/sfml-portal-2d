#include "Wall.hpp"
#include "SFML/Graphics/Rect.hpp"

namespace game {

Wall::Wall() : RigidBody(ObjectClass::wall, "wall", true, 1.f, 0.9f) {}

void Wall::setScale(const sf::Vector2f &scale) {
  const sf::Vector2f oldSizeF(this->getTextureRect().getSize());
  const sf::Vector2i newSizeI(oldSizeF.x * scale.x, oldSizeF.y * scale.y);
  sf::IntRect textureRect({0, 0}, newSizeI);
  this->setTextureRect(textureRect);
  this->RigidBody::setScale(scale);
  this->setOrigin(sf::Vector2f(textureRect.getSize()) * 0.5f);
}

void Wall::setScale(float x, float y) { this->setScale({x, y}); }

void Wall::resetHitbox() {
  this->hitboxes.clear();
  hitboxes.push_back(Hitbox(this->getGlobalBounds().getSize(),
                            this->getOrigin(), this->getPosition()));
}

} // namespace game
