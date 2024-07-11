#include "Logic.hpp"
#include "Emitters.hpp"
#include "RBController.hpp"
#include "RigidBody.hpp"
#include "SFML/System/Sleep.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "Simulation.hpp"
#include <cstddef>
#include <vector>

namespace game {

LogicIns::LogicIns(int targetFps_, int physicsStepsPerFrame_)
    : targetFps(targetFps_), physicsStepsPerFrame(physicsStepsPerFrame_),
      renderMode(renderModes::menuMode), isRunning(true),
      keyStatus(sf::Keyboard::KeyCount, false) {
  Emitters::createInstance();
  Graphics::createInstance();
  Graphics::get().setPlayHandler(std::bind(&LogicIns::handlePlay, this));
  Graphics::get().setSettingsHandler(
      std::bind(&LogicIns::handleSettings, this));
  Graphics::get().setExitHandler(std::bind(&LogicIns::handleExit, this));
  Textures::get().getTexturePointer("wall")->setRepeated(true);
  RBController::createInstance();
}

LogicIns::~LogicIns() {
  RBController::deleteInstance();
  Graphics::deleteInstance();
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
    Graphics::get().render(this->renderMode, rigidBodies);
    // if (this->renderMode == renderModes::gameMode) {
    //   Graphics::get().renderDebug(rigidBodies);
    // }
    sf::sleep(sf::seconds(frameDuration));
  }
}

bool LogicIns::changesKeyStatus(const sf::Event &event) {
  const bool status = event.type == sf::Event::KeyPressed;
  if (this->keyStatus[event.key.code] != status) {
    this->keyStatus[event.key.code] = status;
    return true;
  }
  return false;
}

void LogicIns::handleEvents() {
  sf::Event event;
  while (Graphics::get().pollEvent(event)) {
    switch (event.type) {
    case sf::Event::Closed:
      this->handleExit();
      break;
    case sf::Event::KeyPressed:
    case sf::Event::KeyReleased:
      if (this->changesKeyStatus(event)) {
        Emitters::get().keyboard.emit(
            {.type = event.type, .key = event.key.code});
      }
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
