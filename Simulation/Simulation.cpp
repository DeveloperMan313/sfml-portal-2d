#include "Simulation.hpp"
#include "../Math/Math.hpp"
#include "SFML/System/Vector2.hpp"
#include <cstddef>

namespace game {

const sf::Vector2f Simulation::gravityAcc(0.f, 9.81f);
const float Simulation::collisionShiftCoef = 0.1f;

void Simulation::step(std::vector<RigidBody *> &rigidBodies, float stepSize) {
  const size_t RBsSize = rigidBodies.size();
  for (size_t i = 0; i < RBsSize; ++i) {
    rigidBodies[i]->applyForce(rigidBodies[i]->mass * Simulation::gravityAcc);
    rigidBodies[i]->physicsStep(stepSize);
  }
  Simulation::processCollisions(rigidBodies);
}

void Simulation::processCollisions(std::vector<RigidBody *> &rigidBodies) {
  const size_t RBsSize = rigidBodies.size();
  std::vector<Simulation::collisionInfo> collisions;
  // reserve RBsSize number of collision info to prevent unnecessary allocation
  collisions.reserve(RBsSize);
  // detect all collisions and call handleHitboxesCollision()'s
  for (size_t i = 0; i < RBsSize; ++i) {
    for (size_t j = i + 1; j < RBsSize; ++j) {
      RigidBody &rb1 = *rigidBodies[i], &rb2 = *rigidBodies[j];
      if (rb1.isStatic && rb2.isStatic) {
        continue;
      }
      for (size_t k = 0; k < rb1.hitboxes.size(); ++k) {
        for (size_t l = 0; l < rb2.hitboxes.size(); ++l) {
          if (!rb1.hitboxes[k].intersects(rb2.hitboxes[l])) {
            continue;
          }
          const Hitbox &hb1 = rb1.hitboxes[k], &hb2 = rb2.hitboxes[l];
          const sf::Vector2f normal = hb1.collisionNormal(hb2);
          collisions.push_back({.rb1Idx = i,
                                .rb2Idx = j,
                                .hb1Idx = k,
                                .hb2Idx = l,
                                .normal = normal});
          rb1.handleHitboxesCollision(rb2, hb2, normal);
          rb2.handleHitboxesCollision(rb1, hb1, normal);
        }
      }
    }
  }
  // process collisions
  for (const Simulation::collisionInfo &info : collisions) {
    RigidBody &rb1_ = *rigidBodies[info.rb1Idx],
              &rb2_ = *rigidBodies[info.rb2Idx];
    const Hitbox hb1_ = rb1_.hitboxes[info.hb1Idx],
                 hb2_ = rb2_.hitboxes[info.hb2Idx];
    // if one rb is static with mass=inf, it will be rb1
    RigidBody &rb1 = (rb1_.mass > rb2_.mass) ? rb1_ : rb2_;
    RigidBody &rb2 = (rb1_.mass > rb2_.mass) ? rb2_ : rb1_;
    const Hitbox &hb1 = (rb1_.mass > rb2_.mass) ? hb1_ : hb2_;
    const Hitbox &hb2 = (rb1_.mass > rb2_.mass) ? hb2_ : hb1_;
    processHitboxesCollision(hb1, hb2, rb1, rb2, info.normal);
  }
}

void Simulation::processHitboxesCollision(const Hitbox &hb1, const Hitbox &hb2,
                                          RigidBody &rb1, RigidBody &rb2,
                                          const sf::Vector2f &normal) {
  // assumption: only one rb can be static and it must be rb1
  // v_proj - projection of speed relative to v11
  const float v21_proj = Math::projection(rb2.velocity - rb1.velocity, normal);
  // return if collision is not in active direction
  if (!(hb1.isInActiveDirection(hb2, rb1.getPosition()) &&
        hb2.isInActiveDirection(hb1, rb2.getPosition()))) {
    return;
  }
  // correct positions
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
    rb2.move(deltaShift);
  }
  // return if rbs are flying apart
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
  if (rb1.isStatic) {
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
}

} // namespace game
