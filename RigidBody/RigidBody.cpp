#include "RigidBody.hpp"
#include "SFML/System/Vector2.hpp"
#include <limits>
#include <stdexcept>

namespace game {

RigidBody::RigidBody(const std::string &textureName, const Textures &textures,
                     bool isStatic_, float mass_, float bounciness_)
    : Sprite(textureName, textures), isStatic(isStatic_), force({0.f, 0.f}),
      velocity({0.f, 0.f}), mass(1.f), inverseMass(1.f) {
  if (mass_ <= 0.f) {
    throw std::invalid_argument("mass should be greater than 0");
  }
  if (isStatic_) {
    mass_ = std::numeric_limits<float>::infinity();
  }
  this->setBounciness(bounciness_);
  *const_cast<float *>(&this->mass) = mass_;
  *const_cast<float *>(&this->inverseMass) = 1.f / mass_;
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
  const sf::Vector2f halfDeltaVel =
      this->force * this->inverseMass * stepSize * 0.5f;
  this->velocity += halfDeltaVel;
  this->setPosition(this->getPosition() + this->velocity);
  this->velocity += halfDeltaVel;
  this->force = {0.f, 0.f};
}

void RigidBody::setBounciness(float bounciness_) {
  if (bounciness_ < 0.f || bounciness_ > 1.f) {
    throw std::invalid_argument("bounciness should be between 0 and 1");
  }
  this->bounciness = bounciness_;
}

float RigidBody::getBounciness() const { return this->bounciness; }

} // namespace game
