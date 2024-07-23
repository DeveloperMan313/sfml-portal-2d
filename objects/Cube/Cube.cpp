#include "Cube.hpp"

namespace game {

Cube::Cube()
    : RigidBody(ObjectClass::cube, false, 10.f, 0.5f), sprite("companionCube") {
  this->sprite.setOrigin(this->sprite.getGlobalBounds().getSize() * 0.5f);
  this->addHitboxFromSprite(this->sprite);
}

void Cube::render(Frame &frame) const {
  this->sprite.setPosition(this->getPosition());
  frame.add(&this->sprite);
}

} // namespace game
