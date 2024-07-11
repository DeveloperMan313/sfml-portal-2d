#pragma once

#include "RigidBody.hpp"
#include <vector>

namespace game {

class RBControllerIns {
  friend class Singleton<RBControllerIns>;
  friend class LogicIns;

public:
  ~RBControllerIns();

  const std::vector<RigidBody *> &getRBs() const;

  void addRigidBody(RigidBody *rigidBody);

  RigidBody *getRbById(size_t id) const;

  RigidBody *getRbByClass(ObjectClass objectClass, size_t number) const;

private:
  std::vector<RigidBody *> rigidBodies;
  size_t nextRbId;

  void removeDestroyed();

  void clear();

  void deleteRB(const RigidBody *rigidBody) const;

  void operator delete(void *ptr) noexcept;
};

using RBController = Singleton<RBControllerIns>;

} // namespace game
