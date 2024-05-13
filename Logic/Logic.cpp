#include "Logic.hpp"
#include "../Simulation/Simulation.hpp"
#include "SFML/System/Sleep.hpp"
#include "SFML/System/Time.hpp"
#include <cstddef>

namespace game {

Logic::Logic(int targetFps_, int physicsStepsPerFrame_)
    : targetFps(targetFps_), physicsStepsPerFrame(physicsStepsPerFrame_) {}

Logic::~Logic() {
  for (RigidBody *rb : this->rigidBodies) {
    delete rb;
  }
}

void Logic::addRigidBody(RigidBody *rigidBody) {
  this->sprites.push_back(rigidBody);
  this->rigidBodies.push_back(rigidBody);
}

void Logic::run() {
  const float frameDuration = 1.f / this->targetFps;
  const float physicsTimeStep = frameDuration / this->physicsStepsPerFrame;
  while (true) {
    for (size_t i = 0; i < this->physicsStepsPerFrame; ++i) {
      Simulation::step(this->rigidBodies, physicsTimeStep);
    }
    graphics.render(this->sprites);
    sf::sleep(sf::seconds(frameDuration));
  }
}

} // namespace game
