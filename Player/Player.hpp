#pragma once

#include "../RigidBody/RigidBody.hpp"
#include "SFML/System/Clock.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/Window/Keyboard.hpp"

namespace game {

class Player : public RigidBody {
public:
  bool upIsPressed, leftIsPressed, rightIsPressed;

  Player(const Textures &textures);

  void step(float stepSize) override;

  void tryJump();

  void handleHitboxesCollision(RigidBody &otherRigidBody,
                               const Hitbox &otherHitbox,
                               const sf::Vector2f &normal) override;

  void setKeyState(sf::Keyboard::Key key, bool isPressed);

private:
  bool isStanding;
  sf::Clock coyoteClock;
  static const sf::Time coyoteTime;
  static const float moveSpeed, moveSharpnessCoef, jumpSpeed;
};

} // namespace game
