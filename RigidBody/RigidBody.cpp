#include "RigidBody.hpp"
#include "SFML/System/Vector2.hpp"
#include <stdexcept>

namespace game {

RigidBody::RigidBody(const std::string &textureName, const Textures &textures,
                     float mass_, bool isStatic_)
    : Sprite(textureName, textures), isStatic(isStatic_), force({0.f, 0.f}),
      velocity({0.f, 0.f}) {
  if (mass_ <= 0.f) {
    throw std::invalid_argument("mass should be greater than 0");
  }
  this->mass = mass_;
}

void RigidBody::applyForce(const sf::Vector2f &force) {
  if (this->isStatic) {
    return;
  }
  this->force += force;
}

void RigidBody::step(float stepSize) {
  if (this->isStatic) {
    return;
  }
  const sf::Vector2f halfDeltaVel = this->force / this->mass * stepSize / 2.f;
  this->velocity += halfDeltaVel;
  this->setPosition(this->getPosition() + this->velocity);
  this->velocity += halfDeltaVel;
}

} // namespace game
