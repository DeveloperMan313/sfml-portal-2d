#include "Cube.hpp"

namespace game {

Cube::Cube(const Textures &textures)
    : RigidBody(objectClass::cube, "companionCube", textures, false, 10.f,
                0.5f) {}

} // namespace game
