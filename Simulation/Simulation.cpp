#include "Simulation.hpp"
#include "../Math/Math.hpp"
#include "SFML/System/Vector2.hpp"
#include <cstddef>

namespace game {

const sf::Vector2f Simulation::gravityAcc(0.f, 9.81f);
const float Simulation::collisionShiftCoef = 0.1f;

void Simulation::processCollision(RigidBody &rb1_, RigidBody &rb2_) {
  if (rb1_.isStatic && rb2_.isStatic) {
    return;
  }
  // if one rb is static with mass=inf, it will be rb1
  RigidBody &rb1 = (rb1_.mass > rb2_.mass) ? rb1_ : rb2_;
  RigidBody &rb2 = (rb1_.mass > rb2_.mass) ? rb2_ : rb1_;
  for (const Hitbox &hb1 : rb1.hitboxes) {
    for (const Hitbox &hb2 : rb2.hitboxes) {
      Simulation::processHitboxesCollision(hb1, hb2, rb1, rb2);
    }
  }
}

void Simulation::step(std::vector<RigidBody *> &rigidBodies, float stepSize) {
  const size_t RBsSize = rigidBodies.size();
  for (size_t i = 0; i < RBsSize; ++i) {
    for (size_t j = i + 1; j < RBsSize; ++j) {
      Simulation::processCollision(*rigidBodies[i], *rigidBodies[j]);
    }
  }
  for (size_t i = 0; i < RBsSize; ++i) {
    rigidBodies[i]->applyForce(rigidBodies[i]->mass * Simulation::gravityAcc);
    rigidBodies[i]->step(stepSize);
  }
}

void Simulation::processHitboxesCollision(const Hitbox &hb1, const Hitbox &hb2,
                                          RigidBody &rb1, RigidBody &rb2) {
  // return if hitboxes don't intersect
  if (!hb1.intersects(hb2)) {
    return;
  }
  const sf::Vector2f normal = hb1.collisionNormal(hb2);
  // v_proj - projection of speed relative to v11
  const float v21_proj = Math::projection(rb2.velocity - rb1.velocity, normal);
  // return if collision is not in active direction of a portal
  // while notifying portal of the collision
  const sf::Vector2f rb1PosRelTo2 = rb1.getPosition() - rb2.getPosition();
  if ((rb1.objClass == objectClass::portal ||
       rb2.objClass == objectClass::portal) &&
      (Math::dot(rb1PosRelTo2, hb1.activeDirection) < 0.f ||
       Math::dot(-rb1PosRelTo2, hb2.activeDirection) < 0.f)) {
    if (rb1.objClass == objectClass::portal) {
      rb1.handleHitboxesCollision(rb2, hb2, normal);
    } else {
      rb2.handleHitboxesCollision(rb1, hb1, normal);
    }
    return;
  }
  // return if hitboxes are flying apart
  const sf::Vector2f hb1CenterPosRelTo2 =
      hb1.getCenterPosition() - hb2.getCenterPosition();
  if (Math::projection(hb1CenterPosRelTo2, normal) * v21_proj < 0.f) {
    return;
  }
  const float bounciness = rb1.getBounciness() * rb2.getBounciness();
  const float m1_over_m2 = rb1.mass * rb2.inverseMass;
  // vproj - projection of initial speed
  const float v11proj = Math::projection(rb1.velocity, normal);
  float v12_proj, v22_proj;
  if (rb1.isStatic || rb2.isStatic) {
    // impulse of static body is 0 and velocity projection will flip
    v12_proj = 0.f;
    v22_proj = -v21_proj * sqrt(bounciness);
  } else {
    // general case
    v12_proj =
        (v21_proj + Math::sign(v21_proj) *
                        sqrt(std::abs(v21_proj * ((1.f / m1_over_m2) *
                                                      (bounciness - v21_proj) +
                                                  bounciness)))) /
        (m1_over_m2 + 1);
    v22_proj = v21_proj - m1_over_m2 * v12_proj;
  }
  rb1.velocity +=
      (-Math::projection(rb1.velocity, normal) + v12_proj + v11proj) * normal;
  rb2.velocity +=
      (-Math::projection(rb2.velocity, normal) + v22_proj + v11proj) * normal;
  const sf::Vector2f deltaShift =
      Math::normalize(
          Math::projection(hb2.getCenterPosition() - hb1.getCenterPosition(),
                           normal) *
          normal) *
      Simulation::collisionShiftCoef;
  while (rb1.intersects(rb2)) {
    if (!rb1.isStatic) {
      rb1.move(-deltaShift);
    }
    if (!rb2.isStatic) {
      rb2.move(deltaShift);
    }
  }
  rb1.handleHitboxesCollision(rb2, hb2, normal);
  rb2.handleHitboxesCollision(rb1, hb1, normal);
}

} // namespace game
