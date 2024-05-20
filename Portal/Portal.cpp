#include "Portal.hpp"
#include "../Math/Math.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"
#include <algorithm>

namespace game {

const float Portal::baseCutoffMinGap = 10.f;

Portal::Portal(const Textures &textures, RigidBody &base_,
               const sf::Vector2f &facing_, const portalColor color_)
    : RigidBody(objectClass::portal,
                (color_ == portalColor::blue) ? "portalBlue" : "portalRed",
                textures, true),
      base(base_), facing(facing_), singularityPoint({0.f, 0.f}), color(color_),
      linkedPortal(nullptr), teleportAngle(0.f) {
  const sf::FloatRect thisBounds = this->getGlobalBounds(),
                      baseBounds = base_.getGlobalBounds();
  if (std::abs(this->facing.x) == 1.f) {
    this->setOrigin(this->getOrigin() +
                    sf::Vector2f((baseBounds.width + thisBounds.width) * 0.5f *
                                     -this->facing.x,
                                 0.f));
  }
  if (std::abs(this->facing.y) == 1.f) {
    this->setOrigin(this->getOrigin() +
                    sf::Vector2f((baseBounds.height + thisBounds.width) * 0.5f *
                                     -this->facing.y,
                                 0.f));
    this->rotate(90.f);
  }
}

void Portal::cutHitbox(size_t baseHitboxIdx) {
  const Hitbox &baseHitbox = this->base.hitboxes[baseHitboxIdx];
  const sf::Vector2f hitboxPos = baseHitbox.getCenterPosition();
  const sf::FloatRect hitboxBounds = baseHitbox.getGlobalBounds();
  const float hitboxBottom = hitboxBounds.top + hitboxBounds.height,
              hitboxRight = hitboxBounds.left + hitboxBounds.width;
  float minCenterGap;
  if (std::abs(this->facing.x) == 1.f) {
    minCenterGap =
        Portal::baseCutoffMinGap + this->getGlobalBounds().height * 0.5f;
    this->setPosition(hitboxPos.x - hitboxBounds.width * 0.5f * this->facing.x,
                      std::clamp(this->getPosition().y,
                                 hitboxBounds.top + minCenterGap,
                                 hitboxBottom - minCenterGap));
  }
  if (std::abs(this->facing.y) == 1.f) {
    minCenterGap =
        Portal::baseCutoffMinGap + this->getGlobalBounds().width * 0.5f;
    this->setPosition(
        std::clamp(this->getPosition().x, hitboxBounds.left + minCenterGap,
                   hitboxRight - minCenterGap),
        hitboxPos.y - hitboxBounds.height * 0.5f * this->facing.y);
  }
  this->base.hitboxes.erase(
      std::next(this->base.hitboxes.begin(), baseHitboxIdx));
  const sf::Vector2f thisPos = this->getPosition();
  const sf::FloatRect thisBounds = this->getGlobalBounds();
  const float thisBottom = thisBounds.top + thisBounds.height,
              thisRight = thisBounds.left + thisBounds.width;
  sf::Vector2f thisHitboxSize;
  if (std::abs(this->facing.x) == 1.f) {
    const float topHitboxHeight = thisBounds.top - hitboxBounds.top,
                bottomHitboxHeight = hitboxBottom - thisBottom;
    Hitbox topHitbox(
        sf::Vector2f(hitboxBounds.width, topHitboxHeight),
        sf::Vector2f(hitboxBounds.width * 0.5f, hitboxBounds.height * 0.5f),
        hitboxPos),
        bottomHitbox(
            sf::Vector2f(hitboxBounds.width, bottomHitboxHeight),
            sf::Vector2f(hitboxBounds.width * 0.5f,
                         bottomHitboxHeight - hitboxBounds.height * 0.5f),
            hitboxPos);
    this->base.hitboxes.push_back(topHitbox);
    this->base.hitboxes.push_back(bottomHitbox);
    thisHitboxSize = {hitboxBounds.width, thisBounds.height};
    this->hitboxes[0].rect.setOrigin(
        {thisHitboxSize.x * 0.5f * (1 - this->facing.x),
         thisHitboxSize.y * 0.5f});
  }
  if (std::abs(this->facing.y) == 1.f) {
    const float leftHitboxWidth = thisBounds.left - hitboxBounds.left,
                rightHitboxWidth = hitboxRight - thisRight;
    Hitbox leftHitbox(
        sf::Vector2f(leftHitboxWidth, hitboxBounds.height),
        sf::Vector2f(hitboxBounds.width * 0.5f, hitboxBounds.height * 0.5f),
        hitboxPos),
        rightHitbox(sf::Vector2f(rightHitboxWidth, hitboxBounds.height),
                    sf::Vector2f(rightHitboxWidth - hitboxBounds.width * 0.5f,
                                 hitboxBounds.height * 0.5f),
                    hitboxPos);
    this->base.hitboxes.push_back(leftHitbox);
    this->base.hitboxes.push_back(rightHitbox);
    thisHitboxSize = {thisBounds.width, hitboxBounds.height};
    this->hitboxes[0].rect.setOrigin(
        {thisHitboxSize.x * 0.5f,
         thisHitboxSize.y * 0.5f * (1 - this->facing.y)});
  }
  this->hitboxes[0].setSize(thisHitboxSize);
}

void Portal::link(Portal *portalPtr) {
  if (portalPtr == nullptr) {
    this->hitboxes[0].activeDirection = {0.f, 0.f};
  } else {
    this->hitboxes[0].activeDirection = this->facing;
    this->teleportAngle = Math::fullAngle(-this->facing, portalPtr->facing);
  }
  this->linkedPortal = portalPtr;
}

void Portal::handleHitboxesCollision(RigidBody &otherRigidBody,
                                     const Hitbox &otherHitbox,
                                     const sf::Vector2f &normal) {
  const sf::Vector2f otherPosRel =
      otherRigidBody.getPosition() - this->singularityPoint;
  if (this->linkedPortal == nullptr ||
      Math::dot(otherPosRel, this->facing) < 0.f) {
    return;
  }
  otherRigidBody.setPosition(this->linkedPortal->getPosition() +
                             Math::rotate(otherPosRel, this->teleportAngle));
  otherRigidBody.velocity =
      Math::rotate(otherRigidBody.velocity, this->teleportAngle);
  if (otherRigidBody.objClass != objectClass::player) {
    otherRigidBody.rotate(this->teleportAngle);
  }
}

void Portal::setPosition(const sf::Vector2f &position) {
  const sf::FloatRect baseBounds = this->base.getGlobalBounds();
  this->singularityPoint = position;
  if (std::abs(this->facing.x) == 1.f) {
    this->singularityPoint.x += baseBounds.width * 0.5 * this->facing.x;
  } else {
    this->singularityPoint.y += baseBounds.height * 0.5 * this->facing.y;
  }
  this->RigidBody::setPosition(position);
}

void Portal::setPosition(float x, float y) { this->setPosition({x, y}); }

} // namespace game
