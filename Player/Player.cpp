#include "Player.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <cmath>

namespace game {

const sf::Time Player::coyoteTime = sf::milliseconds(50);

const float Player::moveSpeed = 20.f, Player::moveSharpnessCoef = 1.2f,
            Player::jumpSpeed = 50.f;

Player::Player(const Textures &textures)
    : RigidBody(objectClass::player, "player", textures, false, 50.f, 0.f),
      isStanding(false), upIsPressed(false), leftIsPressed(false),
      rightIsPressed(false) {}

void Player::step(float stepSize) {
  if (this->upIsPressed) {
    this->tryJump();
  }
  if (this->leftIsPressed || this->rightIsPressed) {
    const float moveDirection = 1.f - 2.f * this->leftIsPressed;
    this->applyForce({std::pow(std::abs(Player::moveSpeed * moveDirection -
                                        this->velocity.x),
                               Player::moveSharpnessCoef) *
                          this->mass * moveDirection,
                      0.f});
  }
  this->RigidBody::step(stepSize);
}

void Player::tryJump() {
  if (this->isStanding && coyoteClock.getElapsedTime() < Player::coyoteTime) {
    this->velocity.y = -Player::jumpSpeed;
    this->isStanding = false;
  }
}

void Player::handleHitboxesCollision(RigidBody &otherRigidBody,
                                     const Hitbox &otherHitbox,
                                     const sf::Vector2f &normal) {
  if (normal.y == 1.f && this->hitboxes[0].getCenterPosition().y <
                             otherHitbox.getCenterPosition().y) {
    this->isStanding = true;
    coyoteClock.restart();
  }
}

void Player::setKeyState(sf::Keyboard::Key key, bool isPressed) {
  switch (key) {
  case sf::Keyboard::Key::W:
    this->upIsPressed = isPressed;
    break;
  case sf::Keyboard::Key::A:
    this->leftIsPressed = isPressed;
    break;
  case sf::Keyboard::Key::D:
    this->rightIsPressed = isPressed;
    break;
  default:
    break;
  }
}

} // namespace game
