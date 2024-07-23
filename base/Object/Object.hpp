#pragma once

#include "Frame.hpp"
#include <cstddef>

namespace game {

enum class ObjectType { simple, rigidBody };

enum class ObjectClass { cube, player, portal, portalProjectile, wall };

class Object {
  friend class RBControllerIns;

public:
  const ObjectType objType;
  const ObjectClass objClass;

  Object(ObjectClass objClass_);

  Object(ObjectType objType_, ObjectClass objClass_);

  virtual ~Object() = default;

  virtual void step() = 0;

  virtual void render(Frame &frame) const = 0;

  virtual void subscribe() = 0;

  size_t getId() const;

  bool operator==(const Object &other) const;

  bool operator!=(const Object &other) const;

private:
  size_t id;
};

} // namespace game
