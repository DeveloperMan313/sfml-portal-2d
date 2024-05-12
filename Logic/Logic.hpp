#pragma once

#include "../Graphics/Graphics.hpp"
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

private:
  Graphics graphics;
  std::vector<Sprite *> sprites;
  std::vector<RigidBody *> rigidBodies;
  int targetFps, physicsStepsPerFrame;
};

} // namespace game
