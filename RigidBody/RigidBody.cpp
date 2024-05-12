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
  hitboxes.push_back(Hitbox(this->getGlobalBounds(), {0.f, 0.f}));
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
  this->move(this->velocity * stepSize);
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

void RigidBody::setPosition(const sf::Vector2f &position) {
  this->Sprite::setPosition(position);
  for (Hitbox &hb : this->hitboxes) {
    hb.setPosition(position);
  }
}

void RigidBody::setPosition(float x, float y) { this->setPosition({x, y}); }

void RigidBody::move(const sf::Vector2f &offset) {
  this->setPosition(this->getPosition() + offset);
}

void RigidBody::move(float x, float y) { this->move({x, y}); }

void RigidBody::setSize(const sf::Vector2f &size) {
  const sf::Vector2i textureSize = this->getTextureRect().getSize();
  this->Sprite::setScale({size.x / textureSize.x, size.y / textureSize.y});
  for (Hitbox &hb : this->hitboxes) {
    hb.setSize(size);
  }
}

void RigidBody::setSize(float x, float y) { this->setSize({x, y}); }

void RigidBody::setScale(const sf::Vector2f &scale) {
  const sf::Vector2i textureSize = this->getTextureRect().getSize();
  this->Sprite::setScale(scale);
  for (Hitbox &hb : this->hitboxes) {
    hb.setScale(scale);
  }
}

void RigidBody::setScale(float x, float y) { this->setScale({x, y}); }

bool RigidBody::intersects(const RigidBody &other) {
  for (const Hitbox &hb1 : this->hitboxes) {
    for (const Hitbox &hb2 : other.hitboxes) {
      if (hb1.intersects(hb2)) {
        return true;
      }
    }
  }
  return false;
}

} // namespace game
