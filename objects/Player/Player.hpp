#pragma once

#include "Emitters.hpp"
#include "RigidBody.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Time.hpp"
#include "Sprite.hpp"

namespace game {

class Player : public RigidBody {
public:
  Player();

  void step() override;

  virtual void render(Frame &frame) const override final;

  void handleHitboxesCollision(RigidBody &otherRigidBody, size_t otherHitboxIdx,
                               const sf::Vector2f &normal) override final;

  void handleTeleport(float teleportAngle) override;

  void subscribe() override;

private:
  mutable Sprite spritePlayer;
  bool isStanding, isTryingToJump, isGoingLeft, isGoingRight;
  float lookDirection;
  sf::Clock coyoteClock;
  static const sf::Time coyoteTime;
  static const float moveSpeed, moveSharpnessCoef, jumpSpeed;

  void tryToJump();

  void onKeyboard(const events::Keyboard &event);
};

} // namespace game
