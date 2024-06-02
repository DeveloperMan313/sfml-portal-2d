#include "Cube.hpp"

namespace game {

Cube::Cube(const Textures &textures)
    : RigidBody(ObjectClass::cube, "companionCube", textures, false, 10.f,
                0.5f) {}

} // namespace game
