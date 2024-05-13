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
    rigidBodies[i]->applyForce(rigidBodies[i]->mass * Simulation::gravityAcc);
    rigidBodies[i]->step(stepSize);
    for (size_t j = i + 1; j < RBsSize; ++j) {
      Simulation::processCollision(*rigidBodies[i], *rigidBodies[j]);
    }
  }
}

void Simulation::processHitboxesCollision(const Hitbox &hb1, const Hitbox &hb2,
                                          RigidBody &rb1, RigidBody &rb2) {
  if (!hb1.intersects(hb2)) {
    return;
  }
  const float bounciness = rb1.getBounciness() * rb2.getBounciness();
  const float m1_over_m2 = rb1.mass * rb2.inverseMass;
  const sf::Vector2f normal = hb1.collisionNormal(hb2);
  // vproj - projection of initial speed
  // v_proj - projection of speed relative to v11
  const float v11proj = Math::projection(rb1.velocity, normal);
  const float v21_proj = Math::projection(rb2.velocity - rb1.velocity, normal);
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
          Math::projection(rb2.getPosition() - rb1.getPosition(), normal) *
          normal) *
      Simulation::collisionShiftCoef;
  while (rb1.getGlobalBounds().intersects(rb2.getGlobalBounds())) {
    if (!rb1.isStatic) {
      rb1.move(-deltaShift);
    }
    if (!rb2.isStatic) {
      rb2.move(deltaShift);
    }
  }
}

} // namespace game
