#pragma once

#include "Events.hpp"
#include "RigidBody.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Time.hpp"

namespace game {

class Player : public RigidBody {
public:
  Player();

  void step() override;

  void handleHitboxesCollision(RigidBody &otherRigidBody,
                               const Hitbox &otherHitbox,
                               const sf::Vector2f &normal) override;

  void handleTeleport(float teleportAngle) override;

  void subscribe(events::Emitters &emitters) override;

private:
  bool isStanding, isTryingToJump, isGoingLeft, isGoingRight;
  sf::Clock coyoteClock;
  static const sf::Time coyoteTime;
  static const float moveSpeed, moveSharpnessCoef, jumpSpeed;

  void tryToJump();

  void onKeyboard(const events::Keyboard &event);
};

} // namespace game
