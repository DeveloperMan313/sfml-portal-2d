#pragma once

#include "../Graphics/Graphics.hpp"
#include "../Player/Player.hpp"
#include "../Portal/Portal.hpp"
#include "../RigidBody/RigidBody.hpp"
#include "../Sprite/Sprite.hpp"
#include "../Textures/Textures.hpp"

namespace game {

class Logic {
public:
  Textures textures; // public for testing

  Logic(int targetFps_, int physicsStepsPerFrame_);

  ~Logic();

  void addRigidBody(RigidBody *rigidBody); // public for testing

  void run();

  void handleEvents();

  void removeDestroyed();

  void handlePortalRemoval(const Portal *removed, Portal *other);

  void handlePlay();

  void handleSettings();

  void handleExit();

private:
  Graphics graphics;
  std::vector<Sprite *> sprites;
  std::vector<RigidBody *> rigidBodies;
  int targetFps, physicsStepsPerFrame;
  renderModes renderMode;
  bool isRunning;
  Portal *portalBlue, *portalRed;
  Player *player;
};

} // namespace game
