#include "Portal.hpp"
#include "Math.hpp"
#include "RBController.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"
#include <algorithm>
#include <stdexcept>

namespace game {

const float Portal::baseCutoffMinGap = 10.f;

Portal::Portal(Wall *basePtr_, const sf::Vector2f &facing_,
               const PortalColor color_)
    : RigidBody(ObjectClass::portal, true),
      sprite((color_ == PortalColor::blue) ? "portalBlue" : "portalRed"),
      basePtr(basePtr_), singularityPoint({0.f, 0.f}), color(color_),
      linkedPortal(nullptr), teleportAngle(0.f) {
  const float facingEps = 1e-6;
  if (std::abs(Math::magnitude(facing_) - 1.f) > facingEps) {
    throw std::runtime_error("facing_ should be of magnitude 1");
  }
  const float facingAngle = Math::fullAngle({1.f, 0.f}, facing_);
  if (std::abs(facingAngle - -1.f * Math::PI) < facingEps) {
    this->facing = {-1.f, 0.f};
  } else if (std::abs(facingAngle - -0.5f * Math::PI) < facingEps) {
    this->facing = {0.f, -1.f};
  } else if (std::abs(facingAngle - 0.f) < facingEps) {
    this->facing = {1.f, 0.f};
  } else if (std::abs(facingAngle - 0.5f * Math::PI) < facingEps) {
    this->facing = {0.f, 1.f};
  } else if (std::abs(facingAngle - 1.f * Math::PI) < facingEps) {
    this->facing = {-1.f, 0.f};
  } else {
    throw std::runtime_error("facing_ should be parallel to axis X or Y");
  }
  this->sprite.setOrigin(this->sprite.getGlobalBounds().getSize() * 0.5f);
  this->addHitboxFromSprite(this->sprite);
  const sf::FloatRect thisBounds = this->sprite.getGlobalBounds(),
                      baseBounds = basePtr_->sprite.getGlobalBounds();
  if (std::abs(this->facing.x) == 1.f) {
    this->sprite.setOrigin(this->sprite.getOrigin() +
                           sf::Vector2f((baseBounds.width + thisBounds.width) *
                                            0.5f * -this->facing.x,
                                        0.f));
  }
  if (std::abs(this->facing.y) == 1.f) {
    this->sprite.setOrigin(this->sprite.getOrigin() +
                           sf::Vector2f((baseBounds.height + thisBounds.width) *
                                            0.5f * -this->facing.y,
                                        0.f));
    this->sprite.rotate(90.f);
  }
  this->hitboxes.clear();
  this->addHitboxFromSprite(this->sprite);
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

void Portal::render(Frame &frame) const {
  this->sprite.setPosition(this->getPosition());
  frame.add(&this->sprite);
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
        Portal::baseCutoffMinGap + this->sprite.getGlobalBounds().height * 0.5f;
    this->setPosition(hitboxPos.x - hitboxBounds.width * 0.5f * this->facing.x,
                      std::clamp(this->getPosition().y,
                                 hitboxBounds.top + minCenterGap,
                                 hitboxBottom - minCenterGap));
  }
  if (std::abs(this->facing.y) == 1.f) {
    minCenterGap =
        Portal::baseCutoffMinGap + this->sprite.getGlobalBounds().width * 0.5f;
    this->setPosition(
        std::clamp(this->getPosition().x, hitboxBounds.left + minCenterGap,
                   hitboxRight - minCenterGap),
        hitboxPos.y - hitboxBounds.height * 0.5f * this->facing.y);
  }
  this->sprite.setPosition(this->getPosition());
  this->basePtr->hitboxes.erase(
      std::next(this->basePtr->hitboxes.begin(), baseHitboxIdx));
  const sf::Vector2f thisPos = this->getPosition();
  const sf::FloatRect thisBounds = this->sprite.getGlobalBounds();
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
    this->hitboxes[0].setOrigin(
        {thisHitboxSize.x * 0.5f * (1.f - this->facing.x),
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
    this->hitboxes[0].setOrigin(
        {thisHitboxSize.x * 0.5f,
         thisHitboxSize.y * 0.5f * (1.f - this->facing.y)});
  }
  this->hitboxes[0].setSize(thisHitboxSize);
}

void Portal::handleHitboxesCollision(RigidBody &otherRigidBody,
                                     size_t otherHitboxIdx,
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
  if (otherRigidBody.objClass != ObjectClass::player) {
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
  const sf::FloatRect baseBounds = this->basePtr->sprite.getGlobalBounds();
  this->singularityPoint = position;
  if (std::abs(this->facing.x) == 1.f) {
    this->singularityPoint.x += baseBounds.width * 0.5f * this->facing.x;
  } else {
    this->singularityPoint.y += baseBounds.height * 0.5f * this->facing.y;
  }
  this->RigidBody::setPosition(position);
}

void Portal::setPosition(float x, float y) { this->setPosition({x, y}); }

void Portal::subscribe() {
  Emitters::get().rbAdd.subscribe(
      this->getId(), std::bind(&Portal::onRbAdd, this, std::placeholders::_1));
  Emitters::get().rbRemove.subscribe(
      this->getId(),
      std::bind(&Portal::onRbRemove, this, std::placeholders::_1));
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
  if (event.rbId == this->getId()) {
    return;
  }
  RigidBody *rb = RBController::get().getRbById(event.rbId);
  if (rb->objClass == ObjectClass::portal) {
    Portal *other = dynamic_cast<Portal *>(rb);
    this->link(other);
    other->link(this);
  }
}

void Portal::onRbRemove(const events::RigidBody &event) {
  RigidBody *rb = RBController::get().getRbById(event.rbId);
  if (rb->objClass == ObjectClass::portal && rb == this->linkedPortal) {
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
