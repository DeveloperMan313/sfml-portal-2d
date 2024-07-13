#include "Logic.hpp"
#include "Emitters.hpp"
#include "RBController.hpp"
#include "RigidBody.hpp"
#include "SFML/System/Sleep.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"
#include "Simulation.hpp"
#include "Textures.hpp"
#include "UserInput.hpp"
#include <cstddef>
#include <vector>

namespace game {

LogicIns::LogicIns(int targetFps_, int physicsStepsPerFrame_)
    : targetFps(targetFps_), physicsStepsPerFrame(physicsStepsPerFrame_),
      renderMode(renderModes::menuMode), isRunning(true) {
  Emitters::createInstance();
  Renderer::createInstance();
  Renderer::get().setPlayHandler(std::bind(&LogicIns::handlePlay, this));
  Renderer::get().setSettingsHandler(
      std::bind(&LogicIns::handleSettings, this));
  Renderer::get().setExitHandler(std::bind(&LogicIns::handleExit, this));
  Textures::get().getTexturePointer("wall")->setRepeated(true);
  RBController::createInstance();
  UserInput::createInstance();
}

LogicIns::~LogicIns() {
  UserInput::deleteInstance();
  RBController::deleteInstance();
  Renderer::deleteInstance();
  Emitters::deleteInstance();
}

void LogicIns::run() {
  const float frameDuration = 1.f / this->targetFps;
  const float physicsTimeStep = frameDuration / this->physicsStepsPerFrame;
  const std::vector<RigidBody *> rigidBodies = RBController::get().getRBs();
  while (this->isRunning) {
    if (this->renderMode == renderModes::gameMode) {
      this->handleEvents();
    }
    for (RigidBody *rb : rigidBodies) {
      rb->step();
    }
    for (size_t i = 0; i < this->physicsStepsPerFrame * 10; ++i) { // 10 testing
      Simulation::step(rigidBodies, physicsTimeStep);
    }
    RBController::get().removeDestroyed();
    Renderer::get().render(this->renderMode, rigidBodies);
    // if (this->renderMode == renderModes::gameMode) {
    //   Graphics::get().renderDebug(rigidBodies);
    // }
    sf::sleep(sf::seconds(frameDuration));
  }
}

void LogicIns::handleEvents() {
  sf::Event event;
  while (Renderer::get().pollEvent(event)) {
    if (UserInput::get().checkProcessEvent(event)) {
      continue;
    }
    switch (event.type) {
    case sf::Event::Closed:
      this->handleExit();
      break;
    default:
      break;
    }
  }
}

void LogicIns::handlePlay() { this->renderMode = renderModes::gameMode; }

void LogicIns::handleSettings() {}

void LogicIns::handleExit() { this->isRunning = false; }

void LogicIns::operator delete(void *ptr) noexcept {};

} // namespace game
