#pragma once

#include "Events.hpp"
#include "Hitbox.hpp"
#include "SFML/System/Vector2.hpp"
#include "Sprite.hpp"
#include <vector>

namespace game {

enum class ObjectClass { wall, player, portal, cube };

class RigidBody : public Sprite {
public:
  typedef std::function<RigidBody *(size_t)> getRbByIdT;
  typedef std::function<RigidBody *(ObjectClass, size_t)> getRbByClassT;

  size_t id;
  ObjectClass objectClass;
  bool isStatic, isDestroyed;
  sf::Vector2f velocity;
  const float mass, inverseMass;
  std::vector<Hitbox> hitboxes;

  RigidBody(ObjectClass objectClass_, const std::string &textureName,
            bool isStatic_ = false, float mass_ = 1.f, float bounciness_ = 1.f);

  virtual ~RigidBody() = default;

  virtual void step();

  void applyForce(const sf::Vector2f &force);

  virtual void physicsStep(float stepSize);

  void setBounciness(float bounciness_);

  float getBounciness() const;

  void setPosition(const sf::Vector2f &position);

  void setPosition(float x, float y);

  void move(const sf::Vector2f &offset);

  void move(float x, float y);

  void setSize(const sf::Vector2f &size);

  void setSize(float x, float y);

  void setScale(const sf::Vector2f &scale);

  void setScale(float x, float y);

  bool intersects(const RigidBody &other) const;

  void setCallbacks(const getRbByIdT &getRbById_,
                    const getRbByClassT &getRbByClass_);

  virtual void handleHitboxesCollision(RigidBody &otherRigidBody,
                                       const Hitbox &otherHitbox,
                                       const sf::Vector2f &normal);

  virtual void handleTeleport(float teleportAngle);

  virtual void subscribe(events::Emitters &Emitters);

  bool operator==(const RigidBody &other) const;

  bool operator!=(const RigidBody &other) const;

protected:
  getRbByIdT getRbById;
  getRbByClassT getRbByClass;

private:
  sf::Vector2f force;
  float bounciness;
};

} // namespace game
