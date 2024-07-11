#include "Emitter.hpp"
#include "Events.hpp"
#include "Singleton.hpp"

namespace game {

struct EmittersIns {
  Emitter<events::Keyboard> keyboard;
  Emitter<events::RigidBody> rbAdd;
  Emitter<events::RigidBody> rbRemove;
};

using Emitters = Singleton<EmittersIns>;

} // namespace game