#include "Object.hpp"

namespace game {

Object::Object(ObjectClass objClass_)
    : objType(ObjectType::simple), objClass(objClass_) {}

Object::Object(ObjectType objType_, ObjectClass objClass_)
    : objType(objType_), objClass(objClass_) {}

size_t Object::getId() const { return this->id; }

bool Object::operator==(const Object &other) const {
  return this->id == other.id;
}

bool Object::operator!=(const Object &other) const {
  return this->id != other.id;
}

} // namespace game
