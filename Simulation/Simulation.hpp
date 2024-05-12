#pragma once

#include "../RigidBody/RigidBody.hpp"
#include "SFML/System/Vector2.hpp"

namespace game {

class Simulation {
public:
  static float collisionCorrectionTimeStep;

  static void processCollision(RigidBody &rb1, RigidBody &rb2);

  static void step(std::vector<RigidBody *> &rigidBodies, float stepSize);

private:
  static const sf::Vector2f gravityAcc;

  static void processHitboxesCollision(const Hitbox &hb1, const Hitbox &hb2,
                                       RigidBody &rb1, RigidBody &rb2);
};

} // namespace game
