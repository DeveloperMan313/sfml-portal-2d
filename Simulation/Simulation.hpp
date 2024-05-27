#pragma once

#include "../RigidBody/RigidBody.hpp"
#include "SFML/System/Vector2.hpp"

namespace game {

class Simulation {
public:
  static void step(std::vector<RigidBody *> &rigidBodies, float stepSize);

private:
  struct collisionInfo {
    size_t rb1Idx, rb2Idx, hb1Idx, hb2Idx;
    sf::Vector2f normal;
  };
  static const sf::Vector2f gravityAcc;
  static const float collisionShiftCoef;

  static void processCollisions(std::vector<RigidBody *> &rigidBodies);

  static void processHitboxesCollision(const Hitbox &hb1, const Hitbox &hb2,
                                       RigidBody &rb1, RigidBody &rb2,
                                       const sf::Vector2f &normal);
};

} // namespace game
