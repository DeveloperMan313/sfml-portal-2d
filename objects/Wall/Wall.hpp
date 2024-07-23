#pragma once

#include "RigidBody.hpp"
#include "SFML/System/Vector2.hpp"
#include "Sprite.hpp"

namespace game {

class Wall : public RigidBody {
  friend class Portal;

public:
  Wall();

  void setScale(const sf::Vector2f &scale);

  void setScale(float x, float y);

  void resetHitbox();

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
