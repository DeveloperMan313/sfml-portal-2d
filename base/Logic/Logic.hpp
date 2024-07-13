#pragma once

#include "Renderer.hpp"
#include "Singleton.hpp"

namespace game {

class LogicIns {
  friend class Singleton<LogicIns, int, int>;

public:
  LogicIns(int targetFps_, int physicsStepsPerFrame_);

  ~LogicIns();

  void run();

private:
  int targetFps, physicsStepsPerFrame;
  renderModes renderMode;
  bool isRunning;

  void handleEvents();

  void handlePlay();

  void handleSettings();

  void handleExit();

  void operator delete(void *ptr) noexcept;
};

using Logic = Singleton<LogicIns, int, int>;

} // namespace game
