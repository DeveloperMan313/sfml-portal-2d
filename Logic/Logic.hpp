#pragma once

#include "Emitter.hpp"
#include "Events.hpp"
#include "Graphics.hpp"
#include "RigidBody.hpp"
#include "SFML/Window/Event.hpp"
#include "Sprite.hpp"
#include "Textures.hpp"
#include <vector>

namespace game {

class Logic {
public:
  Logic(int targetFps_, int physicsStepsPerFrame_);

  ~Logic();

  void addRigidBody(RigidBody *rigidBody); // public for testing

  void run();

private:
  Textures textures;
  Graphics graphics;
  std::vector<Sprite *> sprites;
  std::vector<RigidBody *> rigidBodies;
  size_t nextRbId;
  int targetFps, physicsStepsPerFrame;
  renderModes renderMode;
  bool isRunning;
  events::Emitters emitters;
  std::vector<bool> keyStatus;

  bool changesKeyStatus(const sf::Event &event);

  void handleEvents();

  void removeDestroyed();

  RigidBody *getRbById(size_t id);

  RigidBody *getRbByClass(ObjectClass objectClass, size_t number);

  void handlePlay();

  void handleSettings();

  void handleExit();
};

} // namespace game
