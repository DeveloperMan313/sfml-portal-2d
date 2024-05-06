#pragma once

#include "../Sprite/Sprite.hpp"
#include "SFML/System/Vector2.hpp"

namespace game {

class RigidBody : public Sprite {
public:
  bool isStatic;
  sf::Vector2f velocity;

  RigidBody(const std::string &textureName, const Textures &textures,
            float mass_, bool isStatic_ = false);

  void applyForce(const sf::Vector2f &force);

  void step(float stepSize);

private:
  float mass;
  sf::Vector2f force;
};

} // namespace game
