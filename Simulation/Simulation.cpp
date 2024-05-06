#include "Simulation.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"
#include <cstddef>

namespace game {

const sf::Vector2f Simulation::gravity(0.f, 9.81f);
const float Simulation::collisionShiftCoef = 0.001f;

void Simulation::processCollision(RigidBody &rb1, RigidBody &rb2) {
  if (rb1.isStatic && rb2.isStatic ||
      !rb1.getGlobalBounds().intersects(rb2.getGlobalBounds())) {
    return;
  }
  if (!rb1.isStatic) {
    rb1.velocity = {0.f, 0.f};
  }
  if (!rb2.isStatic) {
    rb2.velocity = {0.f, 0.f};
  }
  const sf::Vector2f deltaShift =
      (rb2.getPosition() - rb1.getPosition()) * Simulation::collisionShiftCoef;
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
    rigidBodies[i]->applyForce(Simulation::gravity);
    rigidBodies[i]->step(stepSize);
    for (size_t j = i + 1; j < RBsSize; ++j) {
      Simulation::processCollision(*rigidBodies[i], *rigidBodies[j]);
    }
  }
}

} // namespace game
