#pragma once

#include "../RigidBody/RigidBody.hpp"
#include "SFML/System/Vector2.hpp"

namespace game {

enum class portalColor { blue, red };

class Portal : public RigidBody {
public:
  const portalColor color;
  RigidBody &base;

  Portal(const Textures &textures, RigidBody &base_,
         const sf::Vector2f &facing_, const portalColor color_);

  void cutHitbox(size_t baseHitboxIdx);

  void link(Portal *portalPtr);

  void handleHitboxesCollision(RigidBody &otherRigidBody,
                               const Hitbox &otherHitbox,
                               const sf::Vector2f &normal) override;

  void setPosition(const sf::Vector2f &position);

  void setPosition(float x, float y);

private:
  const sf::Vector2f facing;
  sf::Vector2f singularityPoint;
  Portal *linkedPortal;
  float teleportAngle;
  static const float baseCutoffMinGap;
};

} // namespace game
