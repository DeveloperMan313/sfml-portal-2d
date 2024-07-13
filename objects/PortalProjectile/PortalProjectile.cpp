#include "PortalProjectile.hpp"
#include "Math.hpp"
#include "RigidBody.hpp"
#include <SFML/System/Vector2.hpp>

namespace game {

const float PortalProjectile::speed = 100.f;

PortalProjectile::PortalProjectile(float movementAngle)
    : RigidBody(ObjectClass::portalProjectile, "placeholder", false, 1e-6f,
                0.f) {
  this->velocity = Math::rotate({PortalProjectile::speed, 0.f}, movementAngle);
}

void PortalProjectile::physicsStep(float stepSize) {
  // ignore forces
  this->move(this->velocity * stepSize);
  this->force = {0.f, 0.f};
}

void PortalProjectile::handleHitboxesCollision(RigidBody &otherRigidBody,
                                               const Hitbox &otherHitbox,
                                               const sf::Vector2f &normal) {
  this->isDestroyed = true;
  // TODO portal creation
}

} // namespace game
