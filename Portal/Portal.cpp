#include "Portal.hpp"
#include "../Math/Math.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"
#include <algorithm>

namespace game {

const float Portal::baseCutoffMinGap = 10.f;

Portal::Portal(const Textures &textures, Wall *basePtr_,
               const sf::Vector2f &facing_, const portalColor color_)
    : RigidBody(ObjectClass::portal,
                (color_ == portalColor::blue) ? "portalBlue" : "portalRed",
                textures, true),
      basePtr(basePtr_), facing(facing_), singularityPoint({0.f, 0.f}),
      color(color_), linkedPortal(nullptr), teleportAngle(0.f) {
  const sf::FloatRect thisBounds = this->getGlobalBounds(),
                      baseBounds = basePtr_->getGlobalBounds();
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

Portal::~Portal() {
  if (this->basePtr == nullptr) {
    return;
  }
  if (this->linkedPortal == nullptr ||
      *this->basePtr != *linkedPortal->basePtr) {
    this->basePtr->resetHitbox();
  }
}

void Portal::cutHitbox(size_t baseHitboxIdx) {
  const Hitbox &baseHitbox = this->basePtr->hitboxes[baseHitboxIdx];
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
  this->basePtr->hitboxes.erase(
      std::next(this->basePtr->hitboxes.begin(), baseHitboxIdx));
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
    this->basePtr->hitboxes.push_back(topHitbox);
    this->basePtr->hitboxes.push_back(bottomHitbox);
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
    this->basePtr->hitboxes.push_back(leftHitbox);
    this->basePtr->hitboxes.push_back(rightHitbox);
    thisHitboxSize = {thisBounds.width, hitboxBounds.height};
    this->hitboxes[0].rect.setOrigin(
        {thisHitboxSize.x * 0.5f,
         thisHitboxSize.y * 0.5f * (1 - this->facing.y)});
  }
  this->hitboxes[0].setSize(thisHitboxSize);
}

void Portal::handleHitboxesCollision(RigidBody &otherRigidBody,
                                     const Hitbox &otherHitbox,
                                     const sf::Vector2f &normal) {
  // assuming otherRigidBody has 1 hitbox
  const sf::Vector2f hbPos = otherRigidBody.hitboxes[0].getCenterPosition(),
                     hbPosRelToRb = hbPos - otherRigidBody.getPosition(),
                     singPointRelToHb = this->singularityPoint - hbPos;
  if (this->linkedPortal == nullptr ||
      Math::dot(singPointRelToHb, this->facing) < 0.f ||
      Math::dot(this->getPosition() - hbPos, this->facing) > 0.f ||
      Math::dot(otherRigidBody.velocity, this->facing) > 0.f) {
    return;
  }
  otherRigidBody.setPosition(
      this->linkedPortal->singularityPoint +
      Math::rotate(singPointRelToHb, this->teleportAngle) - hbPosRelToRb);
  otherRigidBody.velocity =
      Math::rotate(otherRigidBody.velocity, this->teleportAngle);
  if (otherRigidBody.objectClass != ObjectClass::player) {
    // reset rotation of 2PI to 0 to avoid floating point error
    if (std::abs(otherRigidBody.getRotation() - Math::PI * 2.f) < 1e-6f) {
      otherRigidBody.setRotation(0.f);
    } else {
      otherRigidBody.rotate(Math::toDegrees(Math::PI * 0.5f));
    }
  }
  otherRigidBody.handleTeleport(this->teleportAngle);
}

void Portal::setPosition(const sf::Vector2f &position) {
  const sf::FloatRect baseBounds = this->basePtr->getGlobalBounds();
  this->singularityPoint = position;
  if (std::abs(this->facing.x) == 1.f) {
    this->singularityPoint.x += baseBounds.width * 0.5f * this->facing.x;
  } else {
    this->singularityPoint.y += baseBounds.height * 0.5f * this->facing.y;
  }
  this->RigidBody::setPosition(position);
}

void Portal::setPosition(float x, float y) { this->setPosition({x, y}); }

void Portal::subscribe(events::Emitters &emitters) {
  emitters.rbAdd.subscribe(
      this->id, std::bind(&Portal::onRbAdd, this, std::placeholders::_1));
  emitters.rbRemove.subscribe(
      this->id, std::bind(&Portal::onRbRemove, this, std::placeholders::_1));
}

void Portal::link(const Portal *portalPtr) {
  if (portalPtr == nullptr) {
    this->hitboxes[0].activeDirection = {0.f, 0.f};
  } else {
    this->hitboxes[0].activeDirection = -this->facing;
    this->teleportAngle = Math::fullAngle(-this->facing, portalPtr->facing);
  }
  this->linkedPortal = portalPtr;
}

void Portal::onRbAdd(const events::RigidBody &event) {
  if (event.rbId == this->id) {
    return;
  }
  RigidBody *rb = this->getRbById(event.rbId);
  if (rb->objectClass == ObjectClass::portal) {
    Portal *other = dynamic_cast<Portal *>(rb);
    this->link(other);
    other->link(this);
  }
}

void Portal::onRbRemove(const events::RigidBody &event) {
  RigidBody *rb = this->getRbById(event.rbId);
  if (rb->objectClass == ObjectClass::portal) {
    Portal *other = dynamic_cast<Portal *>(rb);
    this->link(nullptr);
    other->link(nullptr);
    if (other->basePtr != nullptr && *this->basePtr == *other->basePtr) {
      this->basePtr->resetHitbox();
      this->cutHitbox(0);
    }
  } else if (rb == this->basePtr) {
    this->basePtr = nullptr;
    this->isDestroyed = true;
  }
}

} // namespace game
