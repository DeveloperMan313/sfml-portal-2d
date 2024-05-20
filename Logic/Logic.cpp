#include "Logic.hpp"
#include "../Simulation/Simulation.hpp"
#include "../Wall/Wall.hpp"
#include "SFML/System/Sleep.hpp"
#include "SFML/System/Time.hpp"
#include "SFML/Window/Event.hpp"
#include "SFML/Window/Keyboard.hpp"
#include <cstddef>
#include <vector>

namespace game {

Logic::Logic(int targetFps_, int physicsStepsPerFrame_)
    : targetFps(targetFps_), physicsStepsPerFrame(physicsStepsPerFrame_),
      renderMode(renderModes::menuMode), isRunning(true), portalBlue(nullptr),
      portalRed(nullptr), player(nullptr) {
  this->graphics.setPlayHandler(std::bind(&Logic::handlePlay, this));
  this->graphics.setSettingsHandler(std::bind(&Logic::handleSettings, this));
  this->graphics.setExitHandler(std::bind(&Logic::handleExit, this));
  this->textures.getTexturePointer("wall")->setRepeated(true);
}

Logic::~Logic() {
  this->sprites.clear();
  this->rigidBodies.clear();
  for (RigidBody *rb : this->rigidBodies) {
    delete rb;
  }
}

void Logic::addRigidBody(RigidBody *rigidBody) {
  this->sprites.push_back(rigidBody);
  this->rigidBodies.push_back(rigidBody);
  RigidBody *inserted = this->rigidBodies[this->rigidBodies.size() - 1];
  if (rigidBody->objClass == objectClass::player) {
    this->player = dynamic_cast<Player *>(inserted);
  }
  if (rigidBody->objClass == objectClass::portal) {
    Portal *portal = dynamic_cast<Portal *>(inserted);
    if (portal->color == portalColor::blue) {
      this->portalBlue = portal;
    } else {
      this->portalRed = portal;
    }
    if (this->portalBlue != nullptr && this->portalRed != nullptr) {
      this->portalBlue->link(this->portalRed);
      this->portalRed->link(this->portalBlue);
    }
  }
}

void Logic::run() {
  Player player(this->textures);
  player.setPosition(200.f, 0.f);
  this->addRigidBody(&player);
  const float frameDuration = 1.f / this->targetFps;
  const float physicsTimeStep = frameDuration / this->physicsStepsPerFrame;
  while (this->isRunning) {
    if (this->renderMode == renderModes::gameMode) {
      handleEvents();
    }
    for (size_t i = 0; i < this->physicsStepsPerFrame * 10; ++i) {
      Simulation::step(this->rigidBodies, physicsTimeStep);
    }
    this->removeDestroyed();
    graphics.render(this->renderMode, this->sprites);
    if (this->renderMode == renderModes::gameMode) {
    }
    sf::sleep(sf::seconds(frameDuration));
  }
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
      this->player->setKeyState(event.key.code,
                                event.type == sf::Event::KeyPressed);
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
    if (rigidBody == this->player) {
      this->player = nullptr;
    } else if (rigidBody == this->portalBlue) {
      this->handlePortalRemoval(this->portalBlue, this->portalRed);
      this->portalBlue = nullptr;
    } else if (rigidBody == this->portalRed) {
      this->handlePortalRemoval(this->portalRed, this->portalBlue);
      this->portalRed = nullptr;
    }
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

void Logic::handlePortalRemoval(const Portal *removed, Portal *other) {
  if (other == nullptr) {
    return;
  }
  other->link(nullptr);
  if (&removed->base != &other->base) {
    dynamic_cast<Wall *>(&removed->base)->resetHitbox();
  } else {
    dynamic_cast<Wall *>(&other->base)->resetHitbox();
    other->cutHitbox(0);
  }
}

void Logic::handlePlay() { this->renderMode = renderModes::gameMode; }

void Logic::handleSettings() {}

void Logic::handleExit() { this->isRunning = false; }

} // namespace game
