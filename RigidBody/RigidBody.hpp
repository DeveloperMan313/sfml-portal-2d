#pragma once

#include "../Sprite/Sprite.hpp"
#include "SFML/System/Vector2.hpp"

namespace game {

class RigidBody : public Sprite {
public:
  bool isStatic;
  sf::Vector2f velocity;
  const float mass, inverseMass;

  RigidBody(const std::string &textureName, const Textures &textures,
            bool isStatic_ = false, float mass_ = 1.f, float bounciness_ = 1.f);

  void applyForce(const sf::Vector2f &force);

  void step(float stepSize);

  void setBounciness(float bounciness_);

  float getBounciness();

private:
  sf::Vector2f force;
  float bounciness;
};

} // namespace game
