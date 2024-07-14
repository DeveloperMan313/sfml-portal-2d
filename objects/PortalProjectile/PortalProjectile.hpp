#pragma once

#include "Portal.hpp"
#include "RigidBody.hpp"

namespace game {

class PortalProjectile : public RigidBody {
public:
  PortalProjectile(PortalColor portalColor_, float movementAngle);

  virtual void physicsStep(float stepSize) override final;

  virtual void
  handleHitboxesCollision(RigidBody &otherRigidBody, size_t otherHitboxIdx,
                          const sf::Vector2f &normal) override final;

private:
  const PortalColor portalColor;

  static const float speed;
};

} // namespace game
