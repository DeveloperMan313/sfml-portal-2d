#include "Player.hpp"
#include "../Math/Math.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <functional>

namespace game {

const sf::Time Player::coyoteTime = sf::milliseconds(50);

const float Player::moveSpeed = 50.f, Player::moveSharpnessCoef = 1.2f,
            Player::jumpSpeed = 50.f;

Player::Player(const Textures &textures)
    : RigidBody(ObjectClass::player, "player", textures, false, 50.f, 0.f),
      isStanding(false), isTryingToJump(false), isGoingLeft(false),
      isGoingRight(false) {}

void Player::step(float stepSize) {
  if (this->isTryingToJump) {
    this->tryToJump();
  }
  if (this->isGoingLeft || this->isGoingRight) {
    const float moveDirection = this->isGoingRight - this->isGoingLeft,
                velocityDelta =
                    Player::moveSpeed * moveDirection - this->velocity.x;
    this->applyForce(
        {Math::sign(velocityDelta) *
             std::pow(std::abs(velocityDelta), Player::moveSharpnessCoef) *
             this->mass,
         0.f});
  }
  this->RigidBody::step(stepSize);
}

void Player::tryToJump() {
  if (this->isStanding && coyoteClock.getElapsedTime() < Player::coyoteTime) {
    this->velocity.y = -Player::jumpSpeed;
    this->isStanding = false;
  }
}

void Player::handleHitboxesCollision(RigidBody &otherRigidBody,
                                     const Hitbox &otherHitbox,
                                     const sf::Vector2f &normal) {
  if (normal.y == 1.f &&
      this->hitboxes[0].getCenterPosition().y <
          otherHitbox.getCenterPosition().y &&
      otherHitbox.isInActiveDirection(this->hitboxes[0])) {
    this->isStanding = true;
    coyoteClock.restart();
  }
}

void Player::onKeyboard(const events::Keyboard &event) {
  const bool status = event.type == sf::Event::KeyPressed;
  switch (event.key) {
  case sf::Keyboard::Key::W:
    this->isTryingToJump = status;
    break;
  case sf::Keyboard::Key::A:
    this->isGoingLeft = status;
    break;
  case sf::Keyboard::Key::D:
    this->isGoingRight = status;
    break;
  default:
    break;
  }
}

void Player::handleTeleport(float teleportAngle) {
  const float eps = 1e-6;
  // teleportAngle is PI and going left or right, switch, otherwise do nothing
  if (this->isGoingLeft != this->isGoingRight &&
      std::abs(std::cos(teleportAngle) + 1.f) < eps) {
    this->isGoingLeft = !this->isGoingLeft;
    this->isGoingRight = !this->isGoingRight;
  }
  // teleportAngle is PI/2 or 3PI/2
  else if (std::abs(std::sin(teleportAngle)) > eps) {
    this->isGoingLeft = false;
    this->isGoingRight = false;
  }
}

void Player::subscribe(events::Emitters &emitters) {
  emitters.keyboard.subscribe(
      this->id, std::bind(&Player::onKeyboard, this, std::placeholders::_1));
}

} // namespace game
