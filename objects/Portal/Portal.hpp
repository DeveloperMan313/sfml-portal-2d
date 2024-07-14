#pragma once

#include "Emitters.hpp"
#include "RigidBody.hpp"
#include "SFML/System/Vector2.hpp"
#include "Wall.hpp"

namespace game {

enum class PortalColor { blue, red };

class Portal : public RigidBody {
public:
  const PortalColor color;
  Wall *basePtr;

  Portal(Wall *basePtr_, const sf::Vector2f &facing_, const PortalColor color_);

  ~Portal() override;

  void cutHitbox(size_t baseHitboxIdx);

  void handleHitboxesCollision(RigidBody &otherRigidBody, size_t otherHitboxIdx,
                               const sf::Vector2f &normal) override final;

  void setPosition(const sf::Vector2f &position);

  void setPosition(float x, float y);

  void subscribe() override;

private:
  void link(const Portal *portalPtr);

  void onRbAdd(const events::RigidBody &event);

  void onRbRemove(const events::RigidBody &event);

  sf::Vector2f facing;
  sf::Vector2f singularityPoint;
  const Portal *linkedPortal;
  float teleportAngle;
  static const float baseCutoffMinGap;
};

} // namespace game
