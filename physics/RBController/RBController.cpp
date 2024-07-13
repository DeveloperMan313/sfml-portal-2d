#include "RBController.hpp"
#include "Emitters.hpp"

namespace game {

RBControllerIns::~RBControllerIns() { this->clear(); }

const std::vector<RigidBody *> &RBControllerIns::getRBs() const {
  return this->rigidBodies;
}

void RBControllerIns::addRigidBody(RigidBody *rigidBody) {
  rigidBody->id = this->nextRbId;
  ++this->nextRbId;
  rigidBody->subscribe();
  this->rigidBodies.push_back(rigidBody);
  // rb gets the message of self's addition (may be changed)
  Emitters::get().rbAdd.emit({.rbId = rigidBody->id});
}

RigidBody *RBControllerIns::getRbById(size_t id) const {
  for (RigidBody *rb : this->rigidBodies) {
    if (rb->id == id) {
      return rb;
    }
  }
  return nullptr;
}

RigidBody *RBControllerIns::getRbByClass(ObjectClass objectClass,
                                         size_t number) const {
  for (RigidBody *rb : this->rigidBodies) {
    if (rb->objectClass == objectClass) {
      if (number == 0) {
        return rb;
      }
      --number;
    }
  }
  return nullptr;
}

void RBControllerIns::removeDestroyed() {
  std::erase_if(this->rigidBodies, [this](const game::RigidBody *rb) -> bool {
    if (!rb->isDestroyed) {
      return false;
    }
    this->deleteRB(rb);
    return true;
  });
}

void RBControllerIns::clear() {
  for (const game::RigidBody *rb : this->rigidBodies) {
    this->deleteRB(rb);
  }
  this->rigidBodies.clear();
}

void RBControllerIns::deleteRB(const RigidBody *rigidBody) const {
  Emitters::get().keyboard.unsubscribeOwner(rigidBody->id);
  Emitters::get().mouse.unsubscribeOwner(rigidBody->id);
  Emitters::get().rbAdd.unsubscribeOwner(rigidBody->id);
  Emitters::get().rbRemove.unsubscribeOwner(rigidBody->id);
  // rb doesn't get the message of self's removal, destructor should be used
  Emitters::get().rbRemove.emit({.rbId = rigidBody->id});
  delete rigidBody;
}

void RBControllerIns::operator delete(void *ptr) noexcept {}

} // namespace game
