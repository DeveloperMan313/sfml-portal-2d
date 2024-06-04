#pragma once

#include "RigidBody.hpp"
#include "SFML/System/Vector2.hpp"
#include "Wall.hpp"

namespace game {

enum class portalColor { blue, red };

class Portal : public RigidBody {
public:
  const portalColor color;
  Wall *basePtr;

  Portal(Wall *basePtr_, const sf::Vector2f &facing_, const portalColor color_);

  ~Portal() override;

  void cutHitbox(size_t baseHitboxIdx);

  void handleHitboxesCollision(RigidBody &otherRigidBody,
                               const Hitbox &otherHitbox,
                               const sf::Vector2f &normal) override;

  void setPosition(const sf::Vector2f &position);

  void setPosition(float x, float y);

  void subscribe(events::Emitters &emitters) override;

private:
  void link(const Portal *portalPtr);

  void onRbAdd(const events::RigidBody &event);

  void onRbRemove(const events::RigidBody &event);

  const sf::Vector2f facing;
  sf::Vector2f singularityPoint;
  const Portal *linkedPortal;
  float teleportAngle;
  static const float baseCutoffMinGap;
};

} // namespace game
