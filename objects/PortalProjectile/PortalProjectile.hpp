#pragma once

#include "RigidBody.hpp"

namespace game {

class PortalProjectile : public RigidBody {
public:
  PortalProjectile(float movementAngle);

  virtual void physicsStep(float stepSize) override final;

  virtual void
  handleHitboxesCollision(RigidBody &otherRigidBody, const Hitbox &otherHitbox,
                          const sf::Vector2f &normal) override final;

private:
  static const float speed;
};

} // namespace game
