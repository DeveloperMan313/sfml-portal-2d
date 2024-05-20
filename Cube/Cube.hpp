#pragma once

#include "../RigidBody/RigidBody.hpp"

namespace game {

class Cube : public RigidBody {
public:
  Cube(const Textures &textures);
};

} // namespace game
