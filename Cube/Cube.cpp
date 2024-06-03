#include "Cube.hpp"

namespace game {

Cube::Cube()
    : RigidBody(ObjectClass::cube, "companionCube", false, 10.f, 0.5f) {}

} // namespace game
