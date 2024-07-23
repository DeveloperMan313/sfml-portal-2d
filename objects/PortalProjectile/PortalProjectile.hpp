#pragma once

#include "Frame.hpp"
#include "Portal.hpp"
#include "RigidBody.hpp"
#include "Sprite.hpp"

namespace game {

class PortalProjectile : public RigidBody {
public:
  PortalProjectile(PortalColor portalColor_, float movementAngle);

  inline virtual void step() override final {};

  virtual void physicsStep(float stepSize) override final;

  virtual void render(Frame &frame) const override final;

  inline virtual void subscribe() override final {};

  virtual void
  handleHitboxesCollision(RigidBody &otherRigidBody, size_t otherHitboxIdx,
                          const sf::Vector2f &normal) override final;

  inline virtual void handleTeleport(float teleportAngle) override final {}

private:
  mutable Sprite sprite;
  const PortalColor portalColor;

  static const float speed;
};

} // namespace game
