#include "Wall.hpp"
#include "SFML/Graphics/Rect.hpp"

namespace game {

Wall::Wall() : RigidBody(ObjectClass::wall, true, 1.f, 0.9f), sprite("wall") {
  this->sprite.setOrigin(this->sprite.getGlobalBounds().getSize() * 0.5f);
  this->addHitboxFromSprite(this->sprite);
}

void Wall::setScale(const sf::Vector2f &scale) {
  const sf::Vector2f oldSizeF(this->sprite.getTextureRect().getSize());
  const sf::Vector2i newSizeI(oldSizeF.x * scale.x, oldSizeF.y * scale.y);
  sf::IntRect textureRect({0, 0}, newSizeI);
  this->sprite.setTextureRect(textureRect);
  this->RigidBody::setScale(scale);
  this->sprite.setOrigin(sf::Vector2f(textureRect.getSize()) * 0.5f);
  this->resetHitbox();
}

void Wall::setScale(float x, float y) { this->setScale({x, y}); }

void Wall::resetHitbox() {
  this->hitboxes.clear();
  this->addHitboxFromSprite(this->sprite);
}

void Wall::render(Frame &frame) const {
  this->sprite.setPosition(this->getPosition());
  frame.add(&this->sprite);
}

} // namespace game
