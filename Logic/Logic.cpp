#include "Logic.hpp"
#include "Emitters.hpp"
#include "SFML/System/Sleep.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include "Simulation.hpp"
#include <cstddef>
#include <vector>

namespace game {

LogicIns::LogicIns(int targetFps_, int physicsStepsPerFrame_)
    : nextRbId(1), targetFps(targetFps_),
      physicsStepsPerFrame(physicsStepsPerFrame_),
      renderMode(renderModes::menuMode), isRunning(true),
      keyStatus(sf::Keyboard::KeyCount, false) {
  Emitters::createInstance();
  Graphics::createInstance();
  Graphics::get().setPlayHandler(std::bind(&LogicIns::handlePlay, this));
  Graphics::get().setSettingsHandler(
      std::bind(&LogicIns::handleSettings, this));
  Graphics::get().setExitHandler(std::bind(&LogicIns::handleExit, this));
  Textures::get().getTexturePointer("wall")->setRepeated(true);
}

LogicIns::~LogicIns() {
  for (RigidBody *rb : this->rigidBodies) {
    rb->isDestroyed = true;
  }
  this->removeDestroyed();
  Graphics::deleteInstance();
  Emitters::deleteInstance();
}

void LogicIns::addRigidBody(RigidBody *rigidBody) {
  rigidBody->id = this->nextRbId;
  ++this->nextRbId;
  rigidBody->subscribe();
  rigidBody->setCallbacks(
      std::bind(&LogicIns::getRbById, this, std::placeholders::_1),
      std::bind(&LogicIns::getRbByClass, this, std::placeholders::_1,
                std::placeholders::_2));
  this->rigidBodies.push_back(rigidBody);
  // rb gets the message of self's addition (may be changed)
  Emitters::get().rbAdd.emit({.rbId = rigidBody->id});
}

void LogicIns::run() {
  const float frameDuration = 1.f / this->targetFps;
  const float physicsTimeStep = frameDuration / this->physicsStepsPerFrame;
  while (this->isRunning) {
    if (this->renderMode == renderModes::gameMode) {
      this->handleEvents();
    }
    for (RigidBody *rb : this->rigidBodies) {
      rb->step();
    }
    for (size_t i = 0; i < this->physicsStepsPerFrame * 10; ++i) { // 10 testing
      Simulation::step(this->rigidBodies, physicsTimeStep);
    }
    this->removeDestroyed();
    Graphics::get().render(this->renderMode, this->rigidBodies);
    // if (this->renderMode == renderModes::gameMode) {
    //   Graphics::get().renderDebug(this->rigidBodies);
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

void LogicIns::removeDestroyed() {
  std::vector<bool> idxDestroyed(this->rigidBodies.size(), false);
  for (size_t i = 0; i < this->rigidBodies.size(); ++i) {
    RigidBody *rigidBody = this->rigidBodies[i];
    if (!rigidBody->isDestroyed) {
      continue;
    }
    idxDestroyed[i] = true;
    Emitters::get().keyboard.unsubscribeOwner(rigidBody->id);
    Emitters::get().rbAdd.unsubscribeOwner(rigidBody->id);
    Emitters::get().rbRemove.unsubscribeOwner(rigidBody->id);
    // rb doesn't get the message of self's removal, destructor should be used
    Emitters::get().rbRemove.emit({.rbId = rigidBody->id});
    delete rigidBody;
  }
  size_t i = -1;
  std::erase_if(this->rigidBodies,
                [&idxDestroyed, &i](const game::RigidBody *rb) -> bool {
                  return idxDestroyed[++i];
                });
}

RigidBody *LogicIns::getRbById(size_t id) {
  for (RigidBody *rb : this->rigidBodies) {
    if (rb->id == id) {
      return rb;
    }
  }
  return nullptr;
}

RigidBody *LogicIns::getRbByClass(ObjectClass objectClass, size_t number) {
  for (RigidBody *rb : this->rigidBodies) {
    if (rb->objectClass == objectClass) {
      if (number == 0) {
        return rb;
      }
      --number;
    }
  }
  return nullptr;
}

void LogicIns::handlePlay() { this->renderMode = renderModes::gameMode; }

void LogicIns::handleSettings() {}

void LogicIns::handleExit() { this->isRunning = false; }

void LogicIns::operator delete(void *ptr) noexcept {};

} // namespace game
