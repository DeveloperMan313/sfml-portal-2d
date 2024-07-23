#include "PortalProjectile.hpp"
#include "Math.hpp"
#include "RBController.hpp"
#include "RigidBody.hpp"
#include "Wall.hpp"
#include <cstddef>

namespace game {

const float PortalProjectile::speed = 100.f;

PortalProjectile::PortalProjectile(PortalColor portalColor_,
                                   float movementAngle)
    : RigidBody(ObjectClass::portalProjectile, false, 1e-6f, 0.f),
      sprite("placeholder"), portalColor(portalColor_) {
  this->sprite.setOrigin(this->sprite.getGlobalBounds().getSize() * 0.5f);
  this->addHitboxFromSprite(this->sprite);
  this->velocity = Math::rotate({PortalProjectile::speed, 0.f}, movementAngle);
}

void PortalProjectile::physicsStep(float stepSize) {
  // ignore forces
  this->move(this->velocity * stepSize);
  this->force = {0.f, 0.f};
}

void PortalProjectile::render(Frame &frame) const {
  this->sprite.setPosition(this->getPosition());
  frame.add(&this->sprite);
}

void PortalProjectile::handleHitboxesCollision(RigidBody &otherRigidBody,
                                               size_t otherHitboxIdx,
                                               const sf::Vector2f &normal) {
  if (this->isDestroyed) {
    return;
  }
  this->isDestroyed = true;
  if (otherRigidBody.objClass != ObjectClass::wall) {
    return;
  }
  for (size_t i = 0; i < 2; ++i) {
    RigidBody *rb = RBController::get().getRbByClass(ObjectClass::portal, i);
    if (rb != nullptr &&
        dynamic_cast<Portal *>(rb)->color == this->portalColor) {
      rb->isDestroyed = true;
    }
  }
  const Hitbox otherHitbox = otherRigidBody.hitboxes[otherHitboxIdx];
  const float portalFacingDirection = Math::projection(
      this->getPosition() - otherHitbox.getCenterPosition(), normal);
  const sf::Vector2f portalFacing =
      normal * static_cast<float>(Math::sign(portalFacingDirection));
  Portal *portal = new Portal(dynamic_cast<Wall *>(&otherRigidBody),
                              portalFacing, this->portalColor);
  // displacement along normal from this pos to a center of otherHitbox
  const sf::Vector2f deltaPos =
      normal *
      Math::projection(otherHitbox.getCenterPosition() - this->getPosition(),
                       normal);
  portal->setPosition(this->getPosition() + deltaPos);
  portal->cutHitbox(otherHitboxIdx);
  RBController::get().addRigidBody(portal);
}

} // namespace game
