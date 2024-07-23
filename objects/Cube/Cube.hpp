#pragma once

#include "RigidBody.hpp"
#include "Sprite.hpp"

namespace game {

class Cube : public RigidBody {
public:
  Cube();

  inline virtual void step() override final {};

  virtual void render(Frame &frame) const override final;

  inline virtual void subscribe() override final {};

  inline virtual void
  handleHitboxesCollision(RigidBody &otherRigidBody, size_t otherHitboxIdx,
                          const sf::Vector2f &normal) override final {}

  inline virtual void handleTeleport(float teleportAngle) override final {}

private:
  mutable Sprite sprite;
};

} // namespace game
