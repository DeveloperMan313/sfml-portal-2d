#pragma once

#include "Events.hpp"
#include "Graphics.hpp"
#include "RigidBody.hpp"
#include "SFML/Window/Event.hpp"
#include "Singleton.hpp"
#include <vector>

namespace game {

class LogicIns {
  friend class Singleton<LogicIns, int, int>;

public:
  LogicIns(int targetFps_, int physicsStepsPerFrame_);

  ~LogicIns();

  void addRigidBody(RigidBody *rigidBody); // public for testing

  void run();

private:
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

  void operator delete(void *ptr) noexcept;
};

using Logic = Singleton<LogicIns, int, int>;

} // namespace game
