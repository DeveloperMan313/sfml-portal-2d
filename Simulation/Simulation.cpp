#include "Simulation.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"
#include <cmath>
#include <cstddef>

namespace game {

const sf::Vector2f Simulation::gravityAcc(0.f, 9.81f);
const float Simulation::collisionShiftCoef = 0.001f;

void Simulation::processCollision(RigidBody &rb1_, RigidBody &rb2_) {
  if (rb1_.isStatic && rb2_.isStatic ||
      !rb1_.getGlobalBounds().intersects(rb2_.getGlobalBounds())) {
    return;
  }
  const auto sign = [](float val) -> int { return (0 < val) - (val < 0); };
  const auto magnitude = [](const sf::Vector2f vec) -> float {
    return sqrt(vec.x * vec.x + vec.y * vec.y);
  };
  const auto normalize = [magnitude](const sf::Vector2f vec) -> sf::Vector2f {
    return vec / magnitude(vec);
  };
  const auto projection = [magnitude](const sf::Vector2f vec,
                                      const sf::Vector2f axis) -> float {
    const sf::Vector2f axis_norm = axis / magnitude(axis);
    return vec.x * axis_norm.x + vec.y * axis_norm.y;
  };
  // if one rb is static with mass=inf, it will be rb1
  game::RigidBody &rb1 = (rb1_.mass > rb2_.mass) ? rb1_ : rb2_;
  game::RigidBody &rb2 = (rb1_.mass > rb2_.mass) ? rb2_ : rb1_;
  const sf::Vector2f normal =
      normalize(rb2.getPosition() - rb1.getPosition()); // TODO calculate normal
  // vproj - projection of initial speed
  // v_proj - projection of speed relative to v11
  const float v11proj = projection(rb1.velocity, normal);
  const float v21_proj = projection(rb2.velocity - rb1.velocity, normal);
  const float bounciness = rb1.getBounciness() * rb2.getBounciness();
  const float m1_over_m2 = rb1.mass * rb2.inverseMass;
  const float v12_proj =
      (v21_proj +
       sign(v21_proj) *
           sqrt(abs(v21_proj * ((1.f / m1_over_m2) * (bounciness - v21_proj) +
                                bounciness)))) /
      (m1_over_m2 + 1);
  // impulse of static body is 0 and velocity projection will flip
  const float v22_mproj = (rb1.isStatic || rb2.isStatic)
                              ? -v21_proj * sqrt(bounciness)
                              : v21_proj - m1_over_m2 * v12_proj;
  rb1.velocity +=
      (-projection(rb1.velocity, normal) + v12_proj + v11proj) * normal;
  rb2.velocity +=
      (-projection(rb2.velocity, normal) + v22_mproj + v11proj) * normal;
  const sf::Vector2f deltaShift =
      projection(rb2.getPosition() - rb1.getPosition(), normal) * normal *
      Simulation::collisionShiftCoef;
  while (rb1.getGlobalBounds().intersects(rb2.getGlobalBounds())) {
    if (!rb1.isStatic) {
      rb1.setPosition(rb1.getPosition() - deltaShift);
    }
    if (!rb2.isStatic) {
      rb2.setPosition(rb2.getPosition() + deltaShift);
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

} // namespace game
