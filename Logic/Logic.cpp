#include "Logic.hpp"
#include "../Simulation/Simulation.hpp"
#include "SFML/System/Sleep.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <cstddef>
#include <vector>

namespace game {

Logic::Logic(int targetFps_, int physicsStepsPerFrame_)
    : nextRbId(1), targetFps(targetFps_),
      physicsStepsPerFrame(physicsStepsPerFrame_),
      renderMode(renderModes::menuMode), isRunning(true),
      keyStatus(sf::Keyboard::KeyCount, false) {
  this->graphics.setPlayHandler(std::bind(&Logic::handlePlay, this));
  this->graphics.setSettingsHandler(std::bind(&Logic::handleSettings, this));
  this->graphics.setExitHandler(std::bind(&Logic::handleExit, this));
  Textures::textures->getTexturePointer("wall")->setRepeated(true);
}

Logic::~Logic() {
  for (RigidBody *rb : this->rigidBodies) {
    rb->isDestroyed = true;
  }
  this->removeDestroyed();
}

void Logic::addRigidBody(RigidBody *rigidBody) {
  rigidBody->id = this->nextRbId;
  ++this->nextRbId;
  rigidBody->subscribe(this->emitters);
  rigidBody->setCallbacks(
      std::bind(&Logic::getRbById, this, std::placeholders::_1),
      std::bind(&Logic::getRbByClass, this, std::placeholders::_1,
                std::placeholders::_2));
  this->sprites.push_back(rigidBody);
  this->rigidBodies.push_back(rigidBody);
  // rb gets the message of self's addition (may be changed)
  this->emitters.rbAdd.emit({.rbId = rigidBody->id});
}

void Logic::run() {
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
    graphics.render(this->renderMode, this->sprites);
    // if (this->renderMode == renderModes::gameMode) {
    //   graphics.renderDebug(this->rigidBodies);
    // }
    sf::sleep(sf::seconds(frameDuration));
  }
}

bool Logic::changesKeyStatus(const sf::Event &event) {
  const bool status = event.type == sf::Event::KeyPressed;
  if (this->keyStatus[event.key.code] != status) {
    this->keyStatus[event.key.code] = status;
    return true;
  }
  return false;
}

void Logic::handleEvents() {
  sf::Event event;
  while (this->graphics.pollEvent(event)) {
    switch (event.type) {
    case sf::Event::Closed:
      this->handleExit();
      break;
    case sf::Event::KeyPressed:
    case sf::Event::KeyReleased:
      if (this->changesKeyStatus(event)) {
        this->emitters.keyboard.emit(
            {.type = event.type, .key = event.key.code});
      }
      break;
    default:
      break;
    }
  }
}

void Logic::removeDestroyed() {
  std::vector<bool> idxDestroyed(this->rigidBodies.size(), false);
  for (size_t i = 0; i < this->rigidBodies.size(); ++i) {
    RigidBody *rigidBody = this->rigidBodies[i];
    if (!rigidBody->isDestroyed) {
      continue;
    }
    idxDestroyed[i] = true;
    emitters.keyboard.unsubscribeOwner(rigidBody->id);
    emitters.rbAdd.unsubscribeOwner(rigidBody->id);
    emitters.rbRemove.unsubscribeOwner(rigidBody->id);
    // rb doesn't get the message of self's removal, destructor should be used
    this->emitters.rbRemove.emit({.rbId = rigidBody->id});
    delete rigidBody;
  }
  size_t i = -1;
  std::erase_if(this->rigidBodies,
                [&idxDestroyed, &i](const game::RigidBody *rb) -> bool {
                  return idxDestroyed[++i];
                });
  i = -1;
  std::erase_if(this->sprites,
                [&idxDestroyed, &i](const game::Sprite *sp) -> bool {
                  return idxDestroyed[++i];
                });
}

RigidBody *Logic::getRbById(size_t id) {
  for (RigidBody *rb : this->rigidBodies) {
    if (rb->id == id) {
      return rb;
    }
  }
  return nullptr;
}

RigidBody *Logic::getRbByClass(ObjectClass objectClass, size_t number) {
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

void Logic::handlePlay() { this->renderMode = renderModes::gameMode; }

void Logic::handleSettings() {}

void Logic::handleExit() { this->isRunning = false; }

} // namespace game
